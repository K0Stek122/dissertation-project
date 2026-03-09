#include "AhoCorasickDFA.h"

bool Node::hasChild(const char &key) {
    if (this->children.count(key)) {
        return true;
    } else {
        return false;
    }
}

Node* Node::getChild(const char &key) {
    return this->children[key];
}

void Node::setChild(const char &key, Node* node) {
    this->children[key] = node;
}

void Node::addOutput(const Node &output) {
    this->outputs.insert(output);
}

void Node::copyOutputs(const Node &node) {
    for (auto o : node.outputs) {
        this->outputs.insert(o);
    }
}

bool Node::operator<(const Node& other) const {
    return this < &other;
}