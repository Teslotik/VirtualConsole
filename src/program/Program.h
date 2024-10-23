#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <string>

#include "enums.h"

namespace HVC {

struct Program {
    std::basic_string<char> source;
    Language language;

    Program(Language language, std::basic_string<char> source): language(language), source(source) {}

    virtual void onUpdate() = 0;
    virtual void onAdd() = 0;
    virtual void onRemove() = 0;
};

}

#endif