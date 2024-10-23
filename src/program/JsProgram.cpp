#include <tinyjs/TinyJS_Functions.h>
#include <tinyjs/TinyJS.h>

#include <iostream>
#include <cmath>

#include "enums.h"
#include "JsProgram.h"
#include "ext.h"
#include "utils.inl"

using namespace std;
using namespace HVC;

void registerMath(CTinyJS *js) {
    js->addNative("function print(text)", [](CScriptVar *v, void *userdata) {
        printf("> %s\n", v->getParameter("text")->getString().c_str());
    }, (void*)0);

    js->addNative("function Math.abs(v)", [](CScriptVar *v, void *userdata) {
        v->setReturnVar(new CScriptVar(static_cast<int>(abs(v->getParameter("v")->getDouble()))));
    }, (void*)0);

    js->addNative("function Math.floor(v)", [](CScriptVar *v, void *userdata) {
        v->setReturnVar(new CScriptVar(static_cast<int>(floor(v->getParameter("v")->getDouble()))));
    }, (void*)0);

    js->addNative("function Math.ceil(v)", [](CScriptVar *v, void *userdata) {
        v->setReturnVar(new CScriptVar(ceil(v->getParameter("v")->getDouble())));
    }, (void*)0);

    js->addNative("function Math.round(v)", [](CScriptVar *v, void *userdata) {
        v->setReturnVar(new CScriptVar(round(v->getParameter("v")->getDouble())));
    }, (void*)0);

    js->addNative("function Math.max(a, b)", [](CScriptVar *v, void *userdata) {
        v->setReturnVar(new CScriptVar(max(
            v->getParameter("a")->getDouble(),
            v->getParameter("b")->getDouble()
        )));
    }, (void*)0);

    js->addNative("function Math.min(a, b)", [](CScriptVar *v, void *userdata) {
        v->setReturnVar(new CScriptVar(min(
            v->getParameter("a")->getDouble(),
            v->getParameter("b")->getDouble()
        )));
    }, (void*)0);

    js->addNative("function Math.lerp(x0, y0, x1, y1, t)", [](CScriptVar *v, void *userdata) {
        v->setReturnVar(new CScriptVar(Utils::lerp(
            v->getParameter("x0")->getDouble(),
            v->getParameter("y0")->getDouble(),
            v->getParameter("x1")->getDouble(),
            v->getParameter("y1")->getDouble(),
            static_cast<float>(v->getParameter("t")->getDouble())
        )));
    }, (void*)0);

    js->addNative("function Math.clamp(t, a, b)", [](CScriptVar *v, void *userdata) {
        double t = v->getParameter("t")->getDouble();
        double a = v->getParameter("a")->getDouble();
        double b = v->getParameter("b")->getDouble();
        v->setReturnVar(new CScriptVar(std::min(std::max(t, b), a)));
    }, (void*)0);

    js->addNative("function Math.random()", [](CScriptVar *v, void *userdata) {
        v->setReturnVar(new CScriptVar(
            (float)(rand()) / (float)(RAND_MAX)
        ));
    }, (void*)0);
}

JsProgram::JsProgram(Sandbox *sandbox, std::basic_string<char> source): Program(Language::JavaScript, source), sandbox(sandbox) {
    registerFunctions(&js);
    registerMath(&js);

    /// @note Very ugly solution because we use static polymorphism. See comments in Surface.h
    /// @todo do something with it

    js.addNative("function setPixel(id, x, y, color)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int id = v->getParameter("id")->getInt();
        int x = v->getParameter("x")->getInt();
        int y = v->getParameter("y")->getInt();
        Uint32 color = program->sandbox->palette.indexToColor(v->getParameter("color")->getInt());
        if (auto screen = program->sandbox->isScreen(id); screen) {
            screen->setPixel(x, y, color);
        } else if (auto atlas = program->sandbox->isAtlas(id); atlas) {
            atlas->setPixel(x, y, color);
        } else if (auto canvas = program->sandbox->isCanvas(id); canvas) {
            canvas->setPixel(x, y, color);
        } else if (auto tilemap = program->sandbox->isTilemap(id); tilemap) {
            tilemap->setPixel(x, y, color);
        }
    }, this);

    js.addNative("function setTile(id, tile, x, y)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int id = v->getParameter("id")->getInt();
        int tile = v->getParameter("tile")->getInt();
        int x = v->getParameter("x")->getInt();
        int y = v->getParameter("y")->getInt();
        Uint32 color = program->sandbox->palette.indexToColor(v->getParameter("color")->getInt());
        if (auto tilemap = program->sandbox->isTilemap(id); tilemap) {
            tilemap->setTile(x, y, tile);
        } else {
            cout << "[Error] setTile is not supported for this type of surface" << endl;
        }
    }, this);

    js.addNative("function getPixel(id, x, y)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int id = v->getParameter("id")->getInt();
        int x = v->getParameter("x")->getInt();
        int y = v->getParameter("y")->getInt();
        if (auto screen = program->sandbox->isScreen(id); screen) {
            v->setReturnVar(new CScriptVar(program->sandbox->palette.colorToIndex(screen->getPixel(x, y))));
        } else if (auto atlas = program->sandbox->isAtlas(id); atlas) {
            v->setReturnVar(new CScriptVar(program->sandbox->palette.colorToIndex(atlas->getPixel(x, y))));
        } else if (auto canvas = program->sandbox->isCanvas(id); canvas) {
            v->setReturnVar(new CScriptVar(program->sandbox->palette.colorToIndex(canvas->getPixel(x, y))));
        } else if (auto tilemap = program->sandbox->isTilemap(id); tilemap) {
            v->setReturnVar(new CScriptVar(program->sandbox->palette.colorToIndex(tilemap->getPixel(x, y))));
        }
    }, this);

    js.addNative("function scissor(id, x, y, w, h)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int id = v->getParameter("id")->getInt();
        int x = v->getParameter("x")->getInt();
        int y = v->getParameter("y")->getInt();
        int w = v->getParameter("w")->getInt();
        int h = v->getParameter("h")->getInt();
        if (auto screen = program->sandbox->isScreen(id); screen) {
            screen->scissor(x, y, w, h);
        } else if (auto atlas = program->sandbox->isAtlas(id); atlas) {
            atlas->scissor(x, y, w, h);
        } else if (auto canvas = program->sandbox->isCanvas(id); canvas) {
            canvas->scissor(x, y, w, h);
        } else if (auto tilemap = program->sandbox->isTilemap(id); tilemap) {
            tilemap->scissor(x, y, w, h);
        }
    }, this);

    js.addNative("function scissorAtlas(id, index)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int id = v->getParameter("id")->getInt();
        int index = v->getParameter("index")->getInt();
        if (auto atlas = program->sandbox->isAtlas(id); atlas) {
            atlas->scissor(index);
        } else {
            cout << "[Error] scissorAtlas is not supported for this type of surface" << endl;
        }
    }, this);

    js.addNative("function clearScissor(id)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int id = v->getParameter("id")->getInt();
        if (auto screen = program->sandbox->isScreen(id); screen) {
            screen->clearScissor();
        } else if (auto atlas = program->sandbox->isAtlas(id); atlas) {
            atlas->clearScissor();
        } else if (auto canvas = program->sandbox->isCanvas(id); canvas) {
            canvas->clearScissor();
        } else if (auto tilemap = program->sandbox->isTilemap(id); tilemap) {
            tilemap->clearScissor();
        }
    }, this);

    js.addNative("function clear(id)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int id = v->getParameter("id")->getInt();
        if (auto screen = program->sandbox->isScreen(id); screen) {
            screen->clear();
        } else if (auto atlas = program->sandbox->isAtlas(id); atlas) {
            atlas->clear();
        } else if (auto canvas = program->sandbox->isCanvas(id); canvas) {
            canvas->clear();
        } else if (auto tilemap = program->sandbox->isTilemap(id); tilemap) {
            tilemap->clear();
        }
    }, this);

    js.addNative("function fill(id, color)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int id = v->getParameter("id")->getInt();
        Uint32 color = program->sandbox->palette.indexToColor(v->getParameter("color")->getInt());
        if (auto screen = program->sandbox->isScreen(id); screen) {
            screen->fill(color);
        } else if (auto atlas = program->sandbox->isAtlas(id); atlas) {
            atlas->fill(color);
        } else if (auto canvas = program->sandbox->isCanvas(id); canvas) {
            canvas->fill(color);
        } else if (auto tilemap = program->sandbox->isTilemap(id); tilemap) {
            tilemap->fill(color);
        }
    }, this);

    js.addNative("function line(id, x1, y1, x2, y2, color)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int id = v->getParameter("id")->getInt();
        int x1 = v->getParameter("x1")->getInt();
        int y1 = v->getParameter("y1")->getInt();
        int x2 = v->getParameter("x2")->getInt();
        int y2 = v->getParameter("y2")->getInt();
        Uint32 color = program->sandbox->palette.indexToColor(v->getParameter("color")->getInt());
        if (auto screen = program->sandbox->isScreen(id); screen) {
            screen->line(x1, y1, x2, y2, color);
        } else if (auto atlas = program->sandbox->isAtlas(id); atlas) {
            atlas->line(x1, y1, x2, y2, color);
        } else if (auto canvas = program->sandbox->isCanvas(id); canvas) {
            canvas->line(x1, y1, x2, y2, color);
        } else if (auto tilemap = program->sandbox->isTilemap(id); tilemap) {
            tilemap->line(x1, y1, x2, y2, color);
        }
    }, this);

    js.addNative("function rect(id, x, y, w, h, t, color)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int id = v->getParameter("id")->getInt();
        int x = v->getParameter("x")->getInt();
        int y = v->getParameter("y")->getInt();
        int w = v->getParameter("w")->getInt();
        int h = v->getParameter("h")->getInt();
        int t = v->getParameter("t")->getInt();
        Uint32 color = program->sandbox->palette.indexToColor(v->getParameter("color")->getInt());
        if (auto screen = program->sandbox->isScreen(id); screen) {
            screen->rect(x, y, w, h, t, color);
        } else if (auto atlas = program->sandbox->isAtlas(id); atlas) {
            atlas->rect(x, y, w, h, t, color);
        } else if (auto canvas = program->sandbox->isCanvas(id); canvas) {
            canvas->rect(x, y, w, h, t, color);
        } else if (auto tilemap = program->sandbox->isTilemap(id); tilemap) {
            tilemap->rect(x, y, w, h, t, color);
        }
    }, this);

    js.addNative("function fillRect(id, x, y, w, h, color)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int id = v->getParameter("id")->getInt();
        int x = v->getParameter("x")->getInt();
        int y = v->getParameter("y")->getInt();
        int w = v->getParameter("w")->getInt();
        int h = v->getParameter("h")->getInt();
        Uint32 color = program->sandbox->palette.indexToColor(v->getParameter("color")->getInt());
        if (auto screen = program->sandbox->isScreen(id); screen) {
            screen->fillRect(x, y, w, h, color);
        } else if (auto atlas = program->sandbox->isAtlas(id); atlas) {
            atlas->fillRect(x, y, w, h, color);
        } else if (auto canvas = program->sandbox->isCanvas(id); canvas) {
            canvas->fillRect(x, y, w, h, color);
        } else if (auto tilemap = program->sandbox->isTilemap(id); tilemap) {
            tilemap->fillRect(x, y, w, h, color);
        }
    }, this);

    js.addNative("function ellipse(id, x, y, w, h, color)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int id = v->getParameter("id")->getInt();
        int x = v->getParameter("x")->getInt();
        int y = v->getParameter("y")->getInt();
        int w = v->getParameter("w")->getInt();
        int h = v->getParameter("h")->getInt();
        Uint32 color = program->sandbox->palette.indexToColor(v->getParameter("color")->getInt());
        if (auto screen = program->sandbox->isScreen(id); screen) {
            screen->ellipse(x, y, w, h, color);
        } else if (auto atlas = program->sandbox->isAtlas(id); atlas) {
            atlas->ellipse(x, y, w, h, color);
        } else if (auto canvas = program->sandbox->isCanvas(id); canvas) {
            canvas->ellipse(x, y, w, h, color);
        } else if (auto tilemap = program->sandbox->isTilemap(id); tilemap) {
            tilemap->ellipse(x, y, w, h, color);
        }
    }, this);

    js.addNative("function fillEllipse(id, x, y, w, h, color)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int id = v->getParameter("id")->getInt();
        int x = v->getParameter("x")->getInt();
        int y = v->getParameter("y")->getInt();
        int w = v->getParameter("w")->getInt();
        int h = v->getParameter("h")->getInt();
        Uint32 color = program->sandbox->palette.indexToColor(v->getParameter("color")->getInt());
        if (auto screen = program->sandbox->isScreen(id); screen) {
            screen->fillEllipse(x, y, w, h, color);
        } else if (auto atlas = program->sandbox->isAtlas(id); atlas) {
            atlas->fillEllipse(x, y, w, h, color);
        } else if (auto canvas = program->sandbox->isCanvas(id); canvas) {
            canvas->fillEllipse(x, y, w, h, color);
        } else if (auto tilemap = program->sandbox->isTilemap(id); tilemap) {
            tilemap->fillEllipse(x, y, w, h, color);
        }
    }, this);

    js.addNative("function blit(dst, src, x, y)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int dst = v->getParameter("dst")->getInt();
        int src = v->getParameter("src")->getInt();
        int x = v->getParameter("x")->getInt();
        int y = v->getParameter("y")->getInt();
        if (auto screen = program->sandbox->isScreen(dst); screen) {
            if (auto d = program->sandbox->isScreen(src); d) screen->blit(d, x, y);
            if (auto d = program->sandbox->isAtlas(src); d) screen->blit(d, x, y);
            if (auto d = program->sandbox->isCanvas(src); d) screen->blit(d, x, y);
            if (auto d = program->sandbox->isTilemap(src); d) screen->blit(d, x, y);
        } else if (auto atlas = program->sandbox->isAtlas(dst); atlas) {
            if (auto d = program->sandbox->isScreen(src); d) atlas->blit(d, x, y);
            if (auto d = program->sandbox->isAtlas(src); d) atlas->blit(d, x, y);
            if (auto d = program->sandbox->isCanvas(src); d) atlas->blit(d, x, y);
            if (auto d = program->sandbox->isTilemap(src); d) atlas->blit(d, x, y);
        } else if (auto canvas = program->sandbox->isCanvas(dst); canvas) {
            if (auto d = program->sandbox->isScreen(src); d) canvas->blit(d, x, y);
            if (auto d = program->sandbox->isAtlas(src); d) canvas->blit(d, x, y);
            if (auto d = program->sandbox->isCanvas(src); d) canvas->blit(d, x, y);
            if (auto d = program->sandbox->isTilemap(src); d) canvas->blit(d, x, y);
        } else if (auto tilemap = program->sandbox->isTilemap(dst); tilemap) {
            if (auto d = program->sandbox->isScreen(src); d) tilemap->blit(d, x, y);
            if (auto d = program->sandbox->isAtlas(src); d) tilemap->blit(d, x, y);
            if (auto d = program->sandbox->isCanvas(src); d) tilemap->blit(d, x, y);
            if (auto d = program->sandbox->isTilemap(src); d) tilemap->blit(d, x, y);
        }
    }, this);

    js.addNative("function blitAtlas(dst, src, index)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int dst = v->getParameter("dst")->getInt();
        int src = v->getParameter("src")->getInt();
        int index = v->getParameter("index")->getInt();
        if (auto atlas = program->sandbox->isAtlas(dst); atlas) {
            if (auto d = program->sandbox->isScreen(src); d) atlas->blit(d, index);
            if (auto d = program->sandbox->isAtlas(src); d) atlas->blit(d, index);
            if (auto d = program->sandbox->isCanvas(src); d) atlas->blit(d, index);
            if (auto d = program->sandbox->isTilemap(src); d) atlas->blit(d, index);
        } else {
            cout << "[Error] blitAtlas is not supported for this type of surface" << endl;
        }
    }, this);

    js.addNative("function blitTiles(tilemap, atlas)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int t = v->getParameter("tilemap")->getInt();
        int a = v->getParameter("atlas")->getInt();
        if (auto tilemap = program->sandbox->isTilemap(t); tilemap) {
            if (auto d = program->sandbox->isAtlas(a); d) {
                tilemap->blit(d);
            } else {
                cout << "[Error] blitTiles is not supported for this type of surface" << endl;
            }
        } else {
            cout << "[Error] blitTiles is not supported for this type of surface" << endl;
        }
    }, this);

    js.addNative("function string(id, font, x, y, color, text)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int id = v->getParameter("id")->getInt();
        int font = v->getParameter("font")->getInt();
        int x = v->getParameter("x")->getInt();
        int y = v->getParameter("y")->getInt();
        Uint32 color = program->sandbox->palette.indexToColor(v->getParameter("color")->getInt());
        std::string text = v->getParameter("text")->getString();
        if (auto screen = program->sandbox->isScreen(id); screen) {
            HVC::str(screen, program->sandbox->isAtlas(font), x, y, color, text.c_str());
        } else if (auto atlas = program->sandbox->isAtlas(id); atlas) {
            HVC::str(atlas, program->sandbox->isAtlas(font), x, y, color, text.c_str());
        } else if (auto canvas = program->sandbox->isCanvas(id); canvas) {
            HVC::str(canvas, program->sandbox->isAtlas(font), x, y, color, text.c_str());
        } else if (auto tilemap = program->sandbox->isTilemap(id); tilemap) {
            HVC::str(tilemap, program->sandbox->isAtlas(font), x, y, color, text.c_str());
        }
    }, this);

    js.addNative("function width(id)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int id = v->getParameter("id")->getInt();
        if (auto screen = program->sandbox->isScreen(id); screen) {
            v->setReturnVar(new CScriptVar(screen->w));
        } else if (auto atlas = program->sandbox->isAtlas(id); atlas) {
            v->setReturnVar(new CScriptVar(atlas->w));
        } else if (auto canvas = program->sandbox->isCanvas(id); canvas) {
            v->setReturnVar(new CScriptVar(canvas->w));
        } else if (auto tilemap = program->sandbox->isTilemap(id); tilemap) {
            v->setReturnVar(new CScriptVar(tilemap->w));
        }
    }, this);

    js.addNative("function height(id)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int id = v->getParameter("id")->getInt();
        if (auto screen = program->sandbox->isScreen(id); screen) {
            v->setReturnVar(new CScriptVar(screen->h));
        } else if (auto atlas = program->sandbox->isAtlas(id); atlas) {
            v->setReturnVar(new CScriptVar(atlas->h));
        } else if (auto canvas = program->sandbox->isCanvas(id); canvas) {
            v->setReturnVar(new CScriptVar(canvas->h));
        } else if (auto tilemap = program->sandbox->isTilemap(id); tilemap) {
            v->setReturnVar(new CScriptVar(tilemap->h));
        }
    }, this);

    js.addNative("function size(id)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int id = v->getParameter("id")->getInt();
        if (auto atlas = program->sandbox->isAtlas(id); atlas) {
            v->setReturnVar(new CScriptVar(atlas->size()));
        } else {
            cout << "[Error] size is not supported for this type of surface" << endl;
        }
    }, this);

    js.addNative("function count(id)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int id = v->getParameter("id")->getInt();
        if (auto atlas = program->sandbox->isAtlas(id); atlas) {
            v->setReturnVar(new CScriptVar(atlas->count()));
        } else {
            cout << "[Error] count is not supported for this type of surface" << endl;
        }
    }, this);

    js.addNative("function isButton(btn)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        Button btn = stringToButton(v->getParameter("btn")->getString());
        v->setReturnVar(new CScriptVar(program->sandbox->isButton(btn)));
    }, this);

    js.addNative("function wasButton(btn)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        Button btn = stringToButton(v->getParameter("btn")->getString());
        v->setReturnVar(new CScriptVar(program->sandbox->wasButton(btn)));
    }, this);

    js.addNative("function isOver(x, y, w, h)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        int x = v->getParameter("x")->getInt();
        int y = v->getParameter("y")->getInt();
        int w = v->getParameter("w")->getInt();
        int h = v->getParameter("h")->getInt();
        v->setReturnVar(new CScriptVar(program->sandbox->isOver(x, y, w, h)));
    }, this);

    js.addNative("function isDown(btn, x, y, w, h)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        Button btn = stringToButton(v->getParameter("btn")->getString());
        int x = v->getParameter("x")->getInt();
        int y = v->getParameter("y")->getInt();
        int w = v->getParameter("w")->getInt();
        int h = v->getParameter("h")->getInt();
        v->setReturnVar(new CScriptVar(program->sandbox->isDown(btn, x, y, w, h)));
    }, this);

    js.addNative("function isPressed(btn, x, y, w, h)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        Button btn = stringToButton(v->getParameter("btn")->getString());
        int x = v->getParameter("x")->getInt();
        int y = v->getParameter("y")->getInt();
        int w = v->getParameter("w")->getInt();
        int h = v->getParameter("h")->getInt();
        v->setReturnVar(new CScriptVar(program->sandbox->isPressed(btn, x, y, w, h)));
    }, this);

    js.addNative("function isReleased(btn, x, y, w, h)", [](CScriptVar *v, void *userdata) {
        JsProgram *program = static_cast<JsProgram*>(userdata);
        Button btn = stringToButton(v->getParameter("btn")->getString());
        int x = v->getParameter("x")->getInt();
        int y = v->getParameter("y")->getInt();
        int w = v->getParameter("w")->getInt();
        int h = v->getParameter("h")->getInt();
        v->setReturnVar(new CScriptVar(program->sandbox->isReleased(btn, x, y, w, h)));
    }, this);

    js.root->addChild("black", 0);


    js.root->addChild("black", new CScriptVar(0));
    js.root->addChild("dark", new CScriptVar(1));
    js.root->addChild("light", new CScriptVar(2));
    js.root->addChild("white", new CScriptVar(3));
    js.root->addChild("red", new CScriptVar(4));
    js.root->addChild("redd", new CScriptVar(5));
    js.root->addChild("orange", new CScriptVar(6));
    js.root->addChild("yellow", new CScriptVar(7));
    js.root->addChild("yellowd", new CScriptVar(8));
    js.root->addChild("green", new CScriptVar(9));
    js.root->addChild("greend", new CScriptVar(10));
    js.root->addChild("blue", new CScriptVar(11));
    js.root->addChild("blued", new CScriptVar(12));
    js.root->addChild("purple", new CScriptVar(13));
    js.root->addChild("purpled", new CScriptVar(14));
    js.root->addChild("transparent", new CScriptVar(15));
    js.evaluate("palette = [black, dark, light, white, red, redd, orange, yellow, yellowd, green, greend, blue, blued, purple, purpled, transparent]");
    js.execute(source);

}

void JsProgram::onUpdate() {
    std::string mouseX = to_string(sandbox->mouseX);
    std::string mouseY = to_string(sandbox->mouseY);
    std::string w = to_string(sandbox->getWidth());
    std::string h = to_string(sandbox->getHeight());
    try {
        js.evaluate("mx = " + mouseX + ";");
        js.evaluate("my = " + mouseY + ";");
        js.evaluate("width = " + w + ";");
        js.evaluate("height = " + h + ";");
        js.evaluate("onUpdate();");
        // js.root->addChild("mx", new CScriptVar(1));
    } catch (CScriptException *e) {
        printf("ERROR: %s\n", e->text.c_str());
    }
}

void JsProgram::onAdd() {
    js.evaluate("onAdd();");
}

void JsProgram::onRemove() {
    js.evaluate("onRemove();");
}