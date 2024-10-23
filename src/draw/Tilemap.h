#ifndef TILEMAP_H
#define TILEMAP_H

#include <SDL3/SDL_stdinc.h>
#include <vector>

#include "Surface.h"
#include "Atlas.h"

namespace HVC {

struct Tilemap: Surface<Tilemap> {
    int xTiles;
    int yTiles;

    Tilemap(int w, int h): Surface(w, h) {
        pixels = new Uint32[w * h];
        clear();
        setTiles(22, 8);
    }

    void setTiles(int xTiles, int yTiles) {
        this->xTiles = xTiles;
        this->yTiles = yTiles;
        size_t count = xTiles * yTiles;
        size_t c = tiles.size();
        if (c < count) {
            tiles.resize(xTiles * yTiles);
            for (size_t i = c - 1; i < count; i++) tiles[i] = 0;
        } else {
            tiles.resize(xTiles * yTiles);
        }
    }

    void setTile(int x, int y, int tile) {
        int pos = y * xTiles + x;
        if (pos < tiles.size()) {
            tiles[pos] = tile;
        }
    }

    int xSize() {
        return w / xTiles;
    }

    int ySize() {
        return h / yTiles;
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

    template<typename S>
    void blit(S *surface, int x, int y, Uint32 color = 0xFFFFFFFF) {
        Surface::blit<S>(surface, x, y, color);
    }

    //
    // template<typename S>
    // void blit(S *surface, int id) {
    //     int x = id % xTiles;
    //     int y = id / xTiles;
    //     blit(surface, x * xSize(), y * ySize());
    // }

    void blit(Atlas *atlas) {
        Uint32 *surface = atlas->getPixels();
        int size = atlas->size();
        for (int i = 0; i < xTiles * yTiles; i++) {
            int x = i % xTiles;
            int y = i / xTiles;
            int tile = tiles[i];
            atlas->scissor(tile);
            Surface::blit(atlas, x * size, y * size);
        }
    }

    Uint32 *getPixels() {
        return pixels;
    }

protected:
    std::vector<int> tiles;
    Uint32 *pixels;     // visible area
};

}

#endif