#ifndef EXT_H
#define EXT_H

#include <string>

#include "draw/Atlas.h"

namespace HVC {

template<typename S>
void str(S *surface, Atlas *font, int x, int y, Uint32 color, std::string text) {
    for (int i = 0; i < text.length(); i++) {
        font->scissor(text[i] - ' ');
        surface->blit(font, x + font->size() * i, y, color);
    }
    font->clearScissor();
}

} // namespace HVC

#endif