#include "enums.h"
#include <SDL3/SDL_stdinc.h>
#include "sandbox.h"
#include "draw/tilemap.h"
#include "utils.inl"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <variant>

using HVC::Sandbox;
using HVC::Canvas;
using HVC::Atlas;
using HVC::Tilemap;
using HVC::Program;
using std::string;
using std::stringstream;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;
using std::clamp;
using std::get;
using std::holds_alternative;

Sandbox::Sandbox(int w, int h): w(w), h(h) {
    // for (int i = 0; i < 10; i++) atlases.emplace_back(w, h);
    // for (int i = 0; i < 10; i++) canvases.emplace_back(w, h);
    // for (int i = 0; i < 10; i++) tilemaps.emplace_back(w, h);
    screen = new Canvas(w, h);
    cout << "[Info] Sandbox created" << endl;
}

Sandbox::~Sandbox() {
    // delete surface;
}

Canvas *Sandbox::isScreen(int id) {
    if (id == 0) return screen;
    return nullptr;
}

Atlas *Sandbox::isAtlas(int id) {
    if (id >= 1 && id <= 10) return atlases[id - 1];
    return nullptr;
}

Canvas *Sandbox::Sandbox::isCanvas(int id) {
    if (id >= 11 && id <= 20) return canvases[id - 11];
    return nullptr;
}

Tilemap *Sandbox::isTilemap(int id) {
    if (id >= 21 && id <= 30) return tilemaps[id - 21];
    return nullptr;
}

int Sandbox::getWidth() {
    return w;
}

int Sandbox::getHeight() {
    return h;
}

void Sandbox::init() {

}

void Sandbox::update() {
    event = false;

    for (auto f: delayed) f();
    delayed.clear();

    for (auto it: programs) it->onUpdate();

    for (auto iter = pressed.begin(); iter != pressed.end(); ++iter) {
        released[iter->first] = iter->second;
    }
}

void Sandbox::addProgram(Program *program) {
    delay([this, program]() {
        programs.push_back(program);
        program->onAdd();
    });
}

void Sandbox::removeProgram(Program *program) {
    delay([this, program]() {
        if (auto it = std::find(programs.begin(), programs.end(), program); it != programs.end()) {
            programs.erase(it);
            program->onRemove();
        }
    });
}

Program *Sandbox::createProgram(std::basic_string<wchar_t> source) {
    return nullptr;
}

void Sandbox::playAudio() {

}

void Sandbox::stopAudio() {

}

void Sandbox::getVolume() {

}

void Sandbox::setVolume() {

}

void Sandbox::setMouse(int x, int y) {
    mouseX = x;
    mouseY = y;
}

void Sandbox::setIsPressed(Button b, bool s) {
    pressed[b] = s;
}

bool Sandbox::isButton(Button b) {
    if (b == Button::Unknown) return false;
    return pressed[b];
}

bool Sandbox::wasButton(Button b) {
    if (b == Button::Unknown) return false;
    return released[b];
}

bool Sandbox::isOver(int x, int y, int w, int h) {
    if (x < screen->view.x || x >= screen->view.x + screen->view.w) return false;
    if (y < screen->view.y || y >= screen->view.y + screen->view.h) return false;
    return mouseX > x && mouseY > y && mouseX <= x + w && mouseY <= y + h;
}

bool Sandbox::isDown(Button b, int x, int y, int w, int h) {
    if (event) return false;
    return event |= isOver(x, y, w, h) && isButton(b);
}

bool Sandbox::isPressed(Button b, int x, int y, int w, int h) {
    if (event) return false;
    return event |= isOver(x, y, w, h) && !wasButton(b) && isButton(b);
}

bool Sandbox::isReleased(Button b, int x, int y, int w, int h) {
    if (event) return false;
    return event |= isOver(x, y, w, h) && wasButton(b) && !isButton(b);
}



void Sandbox::print(std::string text) {

}

void Sandbox::delay(std::function<void()> f) {
    delayed.push_back(f);
}


// SurfaceVariant Sandbox::getSurface(int id) {
//     int offset = atlases.size() - 1;
//     if (id <= offset) {
//         return &atlases[id - offset];
//     }

//     offset += canvases.size() - 1;
//     if (id <= offset) {
//         return &canvases[id - offset];
//     }

//     offset += tilemaps.size() - 1;
//     if (id <= offset) {
//         return &tilemaps[id - offset];
//     }
// }