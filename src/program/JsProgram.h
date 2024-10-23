#ifndef JS_PROGRAM_H
#define JS_PROGRAM_H

#include <tinyjs/TinyJS.h>

#include "Program.h"
#include "enums.h"
#include "sandbox.h"

namespace HVC {

// void js_print(CScriptVar *v, void *userdata) {
//     printf("> %s\n", v->getParameter("text")->getString().c_str());
// }

// void js_dump(CScriptVar *v, void *userdata) {
//     CTinyJS *js = (CTinyJS*)userdata;
//     js->root->trace(">  ");
// }

struct JsProgram: Program {
    Sandbox *sandbox;

    JsProgram(Sandbox *sandbox, std::basic_string<char> source);
    
    virtual void onUpdate() override;
    virtual void onAdd() override;
    virtual void onRemove() override;

protected:
    CTinyJS js;
};

} // namespace HVC

#endif