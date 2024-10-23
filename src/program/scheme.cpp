#include "scheme.h"
#include "data.h"
#include "enums.h"
#include "program/program.h"
#include <iostream>
#include <iterator>
#include <string>
#include "sandbox.h"
#include <algorithm>
#include <list>

using std::string;
using std::cout;
using std::endl;
using std::list;
using HVC::Scheme;
using HVC::Node;
using HVC::Socket;

Scheme::Scheme(): Program(Language::Nodes) {
    nodes.push_back(new Node{
        .id = rand(),
        .label = "Add",
        .color = 0xFFFFFFFFu,
        .x = 0,
        .y = 0,
        .inputs = new Socket[2],
        .outputs = new Socket[1],
        .execute = [](Node *node) {

        }
    });
}

void Scheme::set() {

}

void Scheme::init() {

}

void Scheme::update() {

}

void Scheme::place(int x, int y, int w, int h) {
    
}

void Scheme::addNode(int id, const char *label, int x, int y) {
    nodes.push_back(setupNode(new Node{
        .id = id,
        .label = label,
        .x = x,
        .y = y
    }));
}

void Scheme::removeNode(int id) {
    nodes.remove_if([id](Node *node) {
        return node->id == id;
    });
}

void Scheme::replaceNode(int id, const char *label) {
    Node *node = *std::find_if(nodes.begin(), nodes.end(), [id](Node *node) {
        return node->id == id;
    });
    if (node == nullptr) {
        cout << "[Warning] Failed to replace node: node not found" << endl;
    }
    node->label = label;
    delete[] node->inputs;
    delete[] node->outputs;
    setupNode(node);
}

Node *Scheme::setupNode(Node *node) {
    string label = node->label;

    // --------------------------- Data types ---------------------------
    if (label == "Int") {
        node->color = sandbox->palette.blue,
        node->inputs = new Socket[2],
        node->outputs = new Socket[1],
        node->execute = [](Node *node) {

        };
    }

    // --------------------------- Flow control ---------------------------

    // --------------------------- Math ---------------------------
    if (label == "Add") {
        node->color = sandbox->palette.yellow,
        node->inputs = new Socket[2],
        node->outputs = new Socket[1],
        node->execute = [](Node *node) {
            node->outputs[0].value.d = node->inputs[0].value.d + node->inputs[1].value.d;
        };
    }

    return node;
}

void Scheme::build() {
    list<Node*> order;
}