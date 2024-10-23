#ifndef ATLAS_H
#define ATLAS_H

#include <SDL3/SDL_stdinc.h>

#include "Surface.h"

namespace HVC {

struct Atlas: Surface<Atlas> {
    Atlas(int w, int h): Surface(w, h) {
        pixels = new Uint32[w * h];
        clear();
    }

    void load(void *pixels) {
        memcpy(this->pixels, pixels, w * h * sizeof(Uint32));
    }

    int size() {
        return h;
    }

    int count() {
        return w / h;
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

    void scissor(int index) {
        if (index < 0) index = 0;
        if (index >= count()) index = count() - 1;
        view.x = index * size();
        view.y = 0;
        view.w = size();
        view.h = size();
    }

    void clearScissor() {
        view.x = 0;
        view.y = 0;
        view.w = w;
        view.h = h;
    }

    template<typename S>
    void blit(S *surface, int x, int y, Uint32 color = 0xFFFFFFFF) {
        Surface::blit<S>(surface, x, y, color);
    }

    template<typename S>
    void blit(S *surface, int index) {
        blit(surface, index * size(), 0);
    }

    Uint32 *getPixels() {
        return pixels;
    }

protected:
    Uint32 *pixels;
};

} // namespace HVC

#endif