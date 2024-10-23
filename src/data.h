#ifndef DATA_H
#define DATA_H

#include <SDL3/SDL_stdinc.h>
#include <string>
#include <variant>

// #include "draw/Atlas.h"
// #include "draw/Canvas.h"
// #include "draw/Tilemap.h"

namespace HVC {

using std::string;

struct Node;

struct Camera {
    int x, y, w, h;
};

struct Rectangle {
    int x, y, w, h;
};

struct Palette {
    Uint32 black, dark, light, white;
    Uint32 red, redDark;
    Uint32 orange;
    Uint32 yellow, yellowDark;
    Uint32 green, greenDark;
    Uint32 blue, blueDark;
    Uint32 purple, purpleDark;
    Uint32 transparent;

    Uint32 indexToColor(int i) {
        switch (i) {
            case 0: return black;
            case 1: return dark;
            case 2: return light;
            case 3: return white;
            case 4: return red;
            case 5: return redDark;
            case 6: return orange;
            case 7: return yellow;
            case 8: return yellowDark;
            case 9: return green;
            case 10: return greenDark;
            case 11: return blue;
            case 12: return blueDark;
            case 13: return purple;
            case 14: return purpleDark;
            case 15: return transparent;
        }
        return 0;
}

    int colorToIndex(Uint32 c) {
        if (c == black) return 0;
        else if (c == dark) return 1;
        else if (c == light) return 2;
        else if (c == white) return 3;
        else if (c == red) return 4;
        else if (c == redDark) return 5;
        else if (c == orange) return 6;
        else if (c == yellow) return 7;
        else if (c == yellowDark) return 8;
        else if (c == green) return 9;
        else if (c == greenDark) return 10;
        else if (c == blue) return 11;
        else if (c == blueDark) return 12;
        else if (c == purple) return 13;
        else if (c == purpleDark) return 14;
        else if (c == transparent) return 15;
        else if (c == 0x00FFFFFF) return 15;
        return 0;
    }
};

using SurfaceVariant = std::variant<struct Atlas*, struct Canvas*, struct Tilemap*>;
// union SurfaceType {
//     struct Atlas *atlas;
//     struct Canvas *canvas;
//     struct Tilemap *tilemap;
// };

// union Type {
//     bool b;
//     int i;
//     double d;
//     const char *s;
// };

// struct Socket {
//     const char *label;
//     Type value;
//     Uint32 color;
//     int index;
// };

// struct Node {
//     int id;
//     const char *label;
//     Uint32 color = 0xFFFFFFFF;
//     int x;
//     int y;
//     Socket *inputs = nullptr;
//     Socket *outputs = nullptr;
//     void (*execute)(Node *node);

//     ~Node() {
//         delete[] inputs;
//         delete[] outputs;
//     }
// };

// struct Connection {
//     Node *a = nullptr, *b = nullptr;
//     Socket *src = nullptr, *dst = nullptr;
// };

} // namespace HVC

#endif