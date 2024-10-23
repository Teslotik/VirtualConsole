#ifndef SANDBOX_H
#define SANDBOX_H

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <functional>
#include <SDL3/SDL_stdinc.h>

#include "program/Program.h"
#include "data.h"
#include "utils.inl"
#include "Loader.h"
#include "draw/Canvas.h"
#include "draw/Surface.h"
#include "draw/Tilemap.h"
#include "enums.h"

namespace HVC {

struct Sandbox {
    int mouseX = 0, mouseY = 0;

    Loader *loader;
    Palette palette;
    Canvas *screen;

    std::vector<Atlas*> atlases;
    std::vector<Canvas*> canvases;
    std::vector<Tilemap*> tilemaps;
    std::vector<Program*> programs;

    Sandbox(int w, int h);
    ~Sandbox();

    Canvas *isScreen(int id);
    Atlas *isAtlas(int id);
    Canvas *isCanvas(int id);
    Tilemap *isTilemap(int id);

    int getWidth();
    int getHeight();

    // ------------------------------- Program -------------------------------

    // Program
    void init();
    void update();
    void addProgram(Program *program);
    void removeProgram(Program *program);
    Program *createProgram(std::basic_string<wchar_t> source);

    // ------------------------------- Collections -------------------------------
    
    // Audio
    void playAudio();
    void stopAudio();
    void getVolume();
    void setVolume();

    // ------------------------------- I/O -------------------------------

    void setMouse(int x, int y);
    void setIsPressed(Button b, bool s);

    // User controls
    bool isButton(Button b);
    bool wasButton(Button b);
    bool isOver(int x, int y, int w, int h);
    bool isDown(Button b, int x, int y, int w, int h);
    bool isPressed(Button b, int x, int y, int w, int h);
    bool isReleased(Button b, int x, int y, int w, int h);

    // ------------------------------- Utils -------------------------------

    Uint32 indexToColor(int i);
    int colorToIndex(Uint32 c);

    // Debug
    void print(std::string text);

protected:
    int w, h;
    bool event = false;

    std::map<Button, bool> pressed;
    std::map<Button, bool> released;

    std::list<std::function<void()>> delayed;

    void delay(std::function<void()> f);
};

}

#endif