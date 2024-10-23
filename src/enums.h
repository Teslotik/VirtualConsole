#ifndef ENUMS_H
#define ENUMS_H

#include "utils.inl"
#include <iostream>
#include <string.h>
#include <string>

namespace HVC {

using std::string;

enum struct Button {
    Unknown,
    Left,
    Up,
    Right,
    Down,
    A,
    B,
    Select,
    Menu
};

static inline Button stringToButton(string input) {
    // std::cout << input << endl;
    if (input == "l") {
        return Button::Left;
    } else if (input == "u") {
        return Button::Up;
    } else if (input == "r") {
        return Button::Right;
    } else if (input == "d") {
        return Button::Down;
    } else if (input == "a") {
        return Button::A;
    } else if (input == "b") {
        return Button::B;
    } else if (input == "s") {
        return Button::Select;
    } else if (input == "m") {
        return Button::Menu;
    }
    /// @note We splitted conditions blocks for performance reasons
    /// We reduce the number of checks by assumption user uses short names
    if (input == "Left") {
        return Button::Left;
    } else if (input == "Up") {
        return Button::Up;
    } else if (input == "Right") {
        return Button::Right;
    } else if (input == "Down") {
        return Button::Down;
    } else if (input == "A") {
        return Button::A;
    } else if (input == "B") {
        return Button::B;
    } else if (input == "Select") {
        return Button::Select;
    } else if (input == "Menu") {
        return Button::Menu;
    }
    return Button::Unknown;
}

enum struct Language {
    JavaScript,
    Python,
    Haxe,
    Lua,
    Nodes
};

}

#endif