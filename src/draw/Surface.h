#ifndef SURFACE_H
#define SURFACE_H

#include <iostream>
#include <SDL3/SDL_stdinc.h>
#include <string>
#include <cmath>

#include "data.h"

namespace HVC {

/// @note we dont use dynamic polymorphism for performance reasons
/// @brief requires setPixel, getPixel, scissor, clearScissor to be implemented
template<typename T>
struct Surface {
    int w, h;
    Rectangle view;

    Surface(int w, int h): w(w), h(h) {
        view.x = 0;
        view.y = 0;
        view.w = w;
        view.h = h;
    }

    Surface &operator=(const Surface &o) = delete;
    Surface &operator=(const Surface &&o) = delete;

protected:
    Uint32 blend(Uint32 src, Uint32 dst, Uint32 color = 0xFFFFFFFF) {
        float a1 = ((float)(src >> 24 & 0xFF)) / 0xFF;
        float r1 = ((float)(src >> 16 & 0xFF)) / 0xFF;
        float g1 = ((float)(src >> 8 & 0xFF)) / 0xFF;
        float b1 = ((float)(src & 0xFF)) / 0xFF;

        float a2 = ((float)(dst >> 24 & 0xFF)) / 0xFF;
        float r2 = ((float)(dst >> 16 & 0xFF)) / 0xFF;
        float g2 = ((float)(dst >> 8 & 0xFF)) / 0xFF;
        float b2 = ((float)(dst & 0xFF)) / 0xFF;

        float ac = ((float)(color >> 24 & 0xFF)) / 0xFF;
        float rc = ((float)(color >> 16 & 0xFF)) / 0xFF;
        float gc = ((float)(color >> 8 & 0xFF)) / 0xFF;
        float bc = ((float)(color & 0xFF)) / 0xFF;

        // https://github.com/blender/blender/blob/fc8341538a9cd5e0e4c049497277507b82237c9a/source/blender/compositor/realtime_compositor/shaders/library/gpu_shader_compositor_alpha_over.glsl#L23
        float mult = 1.0f - a2;
        float a = (mult * a1 + a2) * ac;
        float r = (mult * r1 + r2) * rc;
        float g = (mult * g1 + g2) * gc;
        float b = (mult * b1 + b2) * bc;

        return ((unsigned)(a * 0xFF) << 24) | ((unsigned)(r * 0xFF) << 16) | ((unsigned)(g * 0xFF) << 8) | ((unsigned)(b * 0xFF));
    }

    void blend(int x, int y, Uint32 dst, Uint32 color = 0xFFFFFFFF) {
        T *base = static_cast<T*>(this);
        Uint32 src = base->getPixel(x, y);
        base->setPixel(x, y, blend(src, dst, color));
    }

public:
    void clear(Uint32 color = 0u) {
        T *base = static_cast<T*>(this);
        for (int i = 0; i < view.w * view.h; i++) {
            int dx = i % view.w;
            int dy = i / view.w;
            base->setPixel(view.x + dx, view.y + dy, color, false);
        }
    }

    void fill(Uint32 color) {
        T *base = static_cast<T*>(this);
        for (int i = 0; i < w * h; ++i) {
            base->setPixel(i % w, i / w, color);
        }
    }

    // http://members.chello.at/easyfilter/bresenham.html
    void line(int x1, int y1, int x2, int y2, Uint32 color) {
        int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
        int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
        int err = dx + dy, e2; /* error value e_xy */

        T *base = static_cast<T*>(this);
        for (;;) {
            base->setPixel(x1, y1, color);
            if (x1 == x2 && y1 == y2) break;
            e2 = 2 * err;
            if (e2 >= dy) { err += dy; x1 += sx; } /* e_xy + e_x > 0 */
            if (e2 <= dx) { err += dx; y1 += sy; } /* e_xy + e_y < 0 */
        }
    }

    void rect(int x, int y, int w, int h, int thickness, Uint32 color) {
        if (w < 0) {
            x += w;
            w *= -1;
        }
        if (h < 0) {
            y += h;
            h *= -1;
        }
        fillRect(x, y, w, thickness, color);
        fillRect(x + w - thickness, y - thickness + thickness, thickness, h, color);
        fillRect(x, y + h - thickness, w, thickness, color);
        fillRect(x, y, thickness, h, color);
    }

    void fillRect(int x, int y, int w, int h, Uint32 color) {
        if (w < 0) {
            x += w + 1;
            w *= -1;
        }
        if (h < 0) {
            y += h + 1;
            h *= -1;
        }
        T *base = static_cast<T*>(this);
        for (int i = 0; i < w * h; ++i) {
            base->setPixel(x + i % w, y + i / w, color);
        }
    }

    // @todo Make the ellipse and the fillEllipse functions look consistent

    // https://schoolofbeginners.blogspot.com/2022/01/midpoint-ellipse-drawing-algorithm-in.html
    // https://github.com/dharmendranamdev/Implementation-of-Midpoint-ellipse-drawing-algorithm-in-OpenGL/blob/master/Ellipse/main.cpp
    void ellipse(int xc, int yc, int w, int h, Uint32 color) {
        T *base = static_cast<T*>(this);
        auto putpixel = [base, color](int x, int y, int xc, int yc) {
            base->setPixel(x + xc, y + yc, color);
            base->setPixel(-x + xc, y + yc, color);
            base->setPixel(x + xc, -y + yc, color);
            base->setPixel(-x + xc, -y + yc, color);
        };

        w = abs(w);
        h = abs(h);

        int x = 0;
        int y = h;
        double region_1 = (h * h) - (w * w) * h + (w * w) * 0.25;
        int dx = 2 * h * h * x;
        int dy = 2 * w * w * y;
        while (dx < dy) {
            putpixel(x, y, xc, yc);
            if (region_1 < 0) {
                x++;
                dx = dx + (h * h) * 2;
                region_1 = region_1 + dx + (h * h);
            } else {
                x++;
                y--;
                dx = dx + (h * h) * 2;
                dy = dy - (w * w) * 2;
                region_1 = region_1 + dx - dy + (h * h);
            }
        }

        double region_2 = (h * h) * (x + 0.5) * (x + 0.5) + (w * w) * (y - 1) * (y - 1) - (w * w * h * h);

        while (y >= 0) {
            putpixel(x, y, xc, yc);
            if (region_2 > 0) {
                y--;
                dy = dy - (w * w) * 2;
                region_2 = region_2 + (w * w) - dy;
            } else {
                y--;
                x++;
                dx = dx + (h * h) * 2;
                dy = dy - (w * w) * 2;
                region_2 = region_2 + dx - dy + (w * w);
            }
        }
    }

    // https://stackoverflow.com/questions/10322341/simple-algorithm-for-drawing-filled-ellipse-in-c-c
    void fillEllipse(int xc, int yc, int w, int h, Uint32 color) {
        T *base = static_cast<T*>(this);
        w = abs(w);
        h = abs(h);
        for (int y = -h; y <= h; y++) {
            for (int x = -w; x <= w; x++) {
                if (x * x * h * h + y * y * w * w <= h * h * w * w) {
                    base->setPixel(xc + x, yc + y, color);
                }
            }
        }
    }

    // template<typename S>
    // void blit(S *surface, int x, int y) {
    //     for (int i = 0; i < view.w * view.h; i++) {
    //         int dx = i % view.w;
    //         int dy = i / view.w;
    //         Uint32 c = surface->getPixel(dx, dy);
    //         blend(x + dx, y + dy, c);
    //     }
    // }

    template<typename S>
    void blit(S *surface, int x, int y, Uint32 color = 0xFFFFFFFF) {
        if (x + surface->view.w < view.x || x > view.x + view.w) return;
        if (y + surface->view.h < view.y || y > view.y + view.h) return;
        for (int i = 0; i < surface->view.w * surface->view.h; i++) {
            int dx = i % surface->view.w;
            int dy = i / surface->view.w;
            Uint32 c = surface->getPixel(surface->view.x + dx, surface->view.y + dy);
            blend(x + dx, y + dy, c, color);
        }
    }
};

} // namespace HVC

#endif