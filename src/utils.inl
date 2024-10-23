#ifndef UTILS_H
#define UTILS_H

#include "SDL3/SDL_stdinc.h"
#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

namespace HVC {

using std::string;
using std::stringstream;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

class Utils {
public:
    static inline string toHexColor(unsigned r, unsigned g, unsigned b, unsigned a) {
        return toHexColor((a << 24) + (r << 16) + (g << 8) + b);
    }

    // https://thispointer.com/convert-an-integer-to-a-hexadecimal-string-in-c/
    static inline string toHexColor(unsigned n) {
        string hex = "00000000";
        int i = static_cast<int>(hex.length()) - 1;
        while (n > 0) {
            int r = n % 16;
            // remainder converted to hexadecimal digit
            char c = (r < 10) ? ('0' + r) : ('a' + r - 10);
            // hexadecimal digit added to start of the string
            hex[i--] = c;
            n /= 16;
        }
        return hex;
    }

    template<typename T>
    static inline int sign(T v) {
        return (v > 0) - (v < 0);
    }

    template<typename T>
    static inline T mix(T a, T b, float t) {
        return a + t * (b - a);
    }

    template<typename T>
    static inline T lerp(T x0, T y0, T x1, T y1, float t) {
        return y0 + (t - x0) * (y1 - y0) / (x1 - x0);
    }

    static inline string read(const char *url) {
        ifstream stream;
        stringstream output;
        try {
            stream.open(url);
            output << stream.rdbuf();
            stream.close();
        } catch (ifstream::failure) {
            cout << "[EXCEPTION] Failed to read file: " << url << endl;
            return nullptr;
        }
        return output.str();
    }

    static inline void write(const char *url, const char *text) {
        ofstream stream;
        try {
            stream.open(url);
            stream << text;
            stream.close();
        } catch (ofstream::failure) {
            cout << "[EXCEPTION] Failed to write file: " << url << endl;
        }
    }

    // // http://members.chello.at/easyfilter/bresenham.html
    // static inline void line(int x1, int y1, int x2, int y2, Uint32 color) {
    //     int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    //     int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    //     int err = dx + dy, e2; /* error value e_xy */

    //     for (;;) {
    //         drawPixel(x1, y1, color);
    //         if (x1 == x2 && y1 == y2) break;
    //         e2 = 2 * err;
    //         if (e2 >= dy) { err += dy; x1 += sx; } /* e_xy + e_x > 0 */
    //         if (e2 <= dx) { err += dx; y1 += sy; } /* e_xy + e_y < 0 */
    //     } 
    // }
};

}

#endif