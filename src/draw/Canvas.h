#ifndef CANVAS_H
#define CANVAS_H

#include <SDL3/SDL_stdinc.h>

#include "Surface.h"
#include "data.h"

namespace HVC {

struct Canvas: Surface<Canvas> {
    Canvas(int w, int h): Surface(w, h) {
        pixels = new Uint32[w * h];
        clear();
    }

    void load(void *pixels) {
        memcpy(this->pixels, pixels, w * h * sizeof(Uint32));
    }

    void setPixel(int x, int y, Uint32 color, bool mix = true) {
        if (x < view.x || x >= view.x + view.w) return;
        if (y < view.y || y >= view.y + view.h) return;
        // cout << std::hex << color << endl;
        if (mix) {
            pixels[y * w + x] = blend(pixels[y * w + x], color);
        } else {
            pixels[y * w + x] = color;
        }
    }

    Uint32 getPixel(int x, int y) {
        if (x < view.x || x >= view.x + view.w) return 0u;
        if (y < view.y || y >= view.y + view.h) return 0u;
        return pixels[y * w + x];
    }

    void scissor(int x, int y, int w, int h) {
        view.x = x;
        view.y = y;
        view.w = w;
        view.h = h;
    }

    void clearScissor() {
        view.x = 0;
        view.y = 0;
        view.w = w;
        view.h = h;
    }

    Uint32 *getPixels() {
        return pixels;
    }

protected:
    Uint32 *pixels;
};

}; // namespace HVC

#endif