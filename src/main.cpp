#include "enums.h"
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include "draw/atlas.h"
#include <SDL3/SDL_surface.h>
#include "data.h"
#include "sandbox.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <algorithm>
#include <SDL3/SDL.h>
#include <TinyJS.h>
#include "program/JsProgram.h"
#include "tinyjs/TinyJS_Functions.h"
#include "utils.inl"

using std::cout;
using std::endl;
using std::min;
using std::max;
using std::clamp;

constexpr int Width = 214;
constexpr int Height = 160;
constexpr int Fps = 60;



class Application {
public:
    int status = 1;
    Uint64 nextTime = 0;
    int fps;
    int width, height;
    HVC::Sandbox *sandbox = nullptr;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *texture = nullptr;

    Application(int width, int height, int fps): width(width), height(height), fps(fps) {
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
            std::cout << "[Error] Failed to initialize SDL: " << SDL_GetError();
            return;
        }

        if (IMG_Init(IMG_INIT_PNG) < 0) {
            // std::cout << "[Error] Failed to initialize IMG: " << IMG_GetError();
            std::cout << "[Error] Failed to initialize IMG: " << endl;
            return;
        }

        window = SDL_CreateWindow("Virtual Console", 1280, 720, SDL_WINDOW_RESIZABLE);
        if (window == nullptr) {
            std::cout << "[Error] Failed to create window";
            return;
        }

        renderer = SDL_CreateRenderer(window, nullptr);
        if (renderer == nullptr) {
            std::cout << "[Error] Failed to create renderer";
            return;
        }

        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

        SDL_ShowWindow(window);

        sandbox = new HVC::Sandbox(width, height);

        HVC::Palette palette{
            .black = 0xFF36373Au,
            .dark = 0xFF898C93u,
            .light = 0xFFCFCFD1u,
            .white = 0xFFF7F7E8u,
            .red = 0xFFF0264Eu,
            .redDark = 0xFF8E2E13u,
            .orange = 0xFFF2B232u,
            .yellow = 0xFFF4F461u,
            .yellowDark = 0xFFEFE03Eu,
            .green = 0xFFADC950u,
            .greenDark = 0xFF91AF4Bu,
            .blue = 0xFF64E1EFu,
            .blueDark = 0xFF3D87DBu,
            .purple = 0xFFE266E2u,
            .purpleDark = 0xFF913C9Eu,
            .transparent = 0x00000000u
        };
        sandbox->palette = palette;

        HVC::Program *program = new HVC::JsProgram(sandbox,
            sandbox->loader->read("script/os.js").c_str()
        );
        sandbox->addProgram(program);

        sandbox->atlases.push_back(sandbox->loader->surface<HVC::Atlas>("assets/font.png"));
        sandbox->atlases.push_back(sandbox->loader->surface<HVC::Atlas>("assets/icons.png"));
        sandbox->atlases.push_back(sandbox->loader->surface<HVC::Atlas>("assets/icons2.png"));
        sandbox->atlases.push_back(sandbox->loader->surface<HVC::Atlas>("assets/tabs.png"));
        sandbox->atlases.push_back(new HVC::Atlas(512, 8));

        sandbox->canvases.push_back(new HVC::Canvas(width, height));
        sandbox->canvases.push_back(new HVC::Canvas(width, height));
        sandbox->canvases.push_back(new HVC::Canvas(width, height));
        sandbox->canvases.push_back(sandbox->loader->surface<HVC::Canvas>("assets/SpriteEditor.png"));
        sandbox->canvases.push_back(sandbox->loader->surface<HVC::Canvas>("assets/TilemapEditor.png"));
        
        sandbox->tilemaps.push_back(new HVC::Tilemap(width, height));

        sandbox->init();
        sandbox->update();

        status = 0;
    }

    virtual ~Application() {
        delete sandbox;
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        // delete sandbox;
        SDL_Quit();
    }

    // @todo
    // void loadConfig
    // void loadArgs
    // void loadCartridge();

    void run() {
        nextTime = SDL_GetTicks();
        SDL_FRect rect{0, 0, 10, 10};
        while (true) {
            // Input
            SDL_Event event;
            if (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) {
                    break;
                }

                int w, h;
                SDL_GetWindowSizeInPixels(window, &w, &h);
                
                // float ratio = 1. / 1.;
                // float ratio = 16. / 9.;
                float ratio = (float)width / (float)height;
                // Fitting the view texture
                if (w / ratio > h) {
                    rect.w = h * ratio;
                    rect.h = (float)h;
                } else {
                    rect.w = (float)w;
                    rect.h = w / ratio;
                }
                // Centering
                rect.x = w / 2.0f - rect.w / 2.0f;
                rect.y = h / 2.0f - rect.h / 2.0f;

                // Input
                // Keyboard
                /// @todo mapping
                if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
                    bool isPressed = event.type == SDL_EVENT_KEY_DOWN;
                    if (event.key.scancode == SDL_SCANCODE_LEFT) sandbox->setIsPressed(HVC::Button::Left, isPressed);
                    if (event.key.scancode == SDL_SCANCODE_UP) sandbox->setIsPressed(HVC::Button::Up, isPressed);
                    if (event.key.scancode == SDL_SCANCODE_RIGHT) sandbox->setIsPressed(HVC::Button::Right, isPressed);
                    if (event.key.scancode == SDL_SCANCODE_DOWN) sandbox->setIsPressed(HVC::Button::Down, isPressed);
                    if (event.key.scancode == SDL_SCANCODE_Z) sandbox->setIsPressed(HVC::Button::A, isPressed);
                    if (event.key.scancode == SDL_SCANCODE_X) sandbox->setIsPressed(HVC::Button::B, isPressed);
                    if (event.key.scancode == SDL_SCANCODE_KP_SPACE) sandbox->setIsPressed(HVC::Button::Select, isPressed);
                    if (event.key.scancode == SDL_SCANCODE_TAB) sandbox->setIsPressed(HVC::Button::Menu, isPressed);
                }
                // Mouse
                if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                    bool isPressed = event.type == SDL_EVENT_MOUSE_BUTTON_DOWN;
                    if (event.button.button == SDL_BUTTON_LEFT) sandbox->setIsPressed(HVC::Button::A, isPressed);
                    if (event.button.button == SDL_BUTTON_RIGHT) sandbox->setIsPressed(HVC::Button::B, isPressed);
                }
                float x, y;
                SDL_GetMouseState(&x, &y);
                sandbox->setMouse(
                    clamp((int)HVC::Utils::lerp(rect.x, 0.f, rect.x + rect.w, (float)width, x), 0, width - 1),
                    clamp((int)HVC::Utils::lerp(rect.y, 0.f, rect.y + rect.h, (float)height, y), 0, height - 1)
                );
            }

            sandbox->update();

            SDL_RenderClear(renderer);
            SDL_UpdateTexture(texture, nullptr, sandbox->screen->getPixels(), sizeof(Uint32) * sandbox->getWidth());
            SDL_RenderTexture(renderer, texture, nullptr, &rect);
            SDL_RenderPresent(renderer);


            // Constant fps
            // https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidetimeexamples.html
            Uint64 now = SDL_GetTicks();
            if (nextTime > now) {
                SDL_Delay(static_cast<Uint32>(nextTime - now));
                nextTime += fps;
            }
        }
    }
};

class WindowsApplication: public Application {
public:
    WindowsApplication(int width, int height, int fps): Application(width, height, fps) {}
};

int main(int argc, char *argv[]) {

    cout << "Hello, worldssszzq!" << endl;

    Application *app = new WindowsApplication(Width, Height, Fps);
    // SDL_HideCursor();
    app->run();

    delete app;

    return 0;
}