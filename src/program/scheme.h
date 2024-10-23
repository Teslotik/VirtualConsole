#ifndef SCHEME_H
#define SCHEME_H

#include "SDL3/SDL_stdinc.h"
#include "program/program.h"
#include <list>
#include <string>

namespace HVC {

struct Node;
struct Connection;

using std::list;
using std::string;

class Scheme: public Program {
public:
    list<Node*> nodes;
    list<Connection*> connections;

    // Scheme() = default;
    Scheme();

    void set();

    void init() override;
    void update() override;
    void place(int x, int y, int w, int h) override;

    void addNode(int id, const char *label, int x, int y);
    void removeNode(int id);
    void replaceNode(int id, const char *label);

protected:
    Node *setupNode(Node *node);
    void build();
};

}

#endif