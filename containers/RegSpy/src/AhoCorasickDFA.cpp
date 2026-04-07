#include "AhoCorasickDFA.h"

bool FoundString::operator==(const FoundString& rhs) const {
    return this->pos == rhs.pos && this->val == rhs.val;
}

bool Node::hasChild(const char &key) {
    if (this->children.count(key)) {
        return true;
    } else {
        return false;
    }
}

Node* Node::getChild(const char &key) {
    auto it = this->children.find(key);
    if (it != this->children.end()) {
        return it->second.get();
    }
    return nullptr;
}

void Node::setChild(const char &key, std::unique_ptr<Node> node) {
    this->children[key] = std::move(node);
}

void Node::addOutput(const std::string& output) {
    this->outputs.insert(output);
}

void Node::copyOutputs(const Node &node) {
    for (auto o : node.outputs) {
        this->outputs.insert(o);
    }
}

AhoCorasick::AhoCorasick(std::vector<std::string> patterns) {
    // COnstruct the Trie
    Node* currNode = &this->root;
    for (auto pattern : patterns) {
        for (auto ch : pattern) {
            char key = ch;
            if (!currNode->hasChild(key)) {
                currNode->setChild(key, std::unique_ptr<Node>(new Node()));
            }
            currNode = currNode->getChild(key);
        }
        currNode->addOutput(pattern);
        currNode = &this->root;
    }
    
    // Failure Link
    this->root.failureLink = &this->root; // Fallback on failure link
    std::queue<Node*> queue;
    
    for (auto& child : this->root.children) {
        // child.first is the key (char)
        // child.second is the value (Node*)
        child.second->failureLink = &this->root;
        queue.push(child.second.get());
    }
    
    while (queue.size() != 0) {
        currNode = queue.front();
        queue.pop();
        
        for (auto& child : currNode->children) {
            //child.first is the key (char)
            //child.second is the value (Node*)
            queue.push(child.second.get());
            Node* n = currNode->failureLink;
            while (n != &this->root and !n->hasChild(child.first)) {
                n = n->failureLink;
            }

            if (n->getChild(child.first)) {
                child.second->failureLink = n->getChild(child.first);
            } else {
                child.second->failureLink = &this->root;
            }
            
            child.second->copyOutputs(*child.second->failureLink);
        }
    }
}

std::vector<FoundString> AhoCorasick::search(const std::string& text)
{
    std::vector<FoundString> found = {};
    auto state = &this->root;
    int i = 0;
    while (i < text.size()) {
        auto c = text[i];
        if (state->hasChild(c)) {
            state = state->getChild(c);
            i++;
            if (state->outputs.size() > 0) {
                for (auto output : state->outputs) {
                    FoundString foundString;
                    foundString.pos = i - output.size();
                    foundString.val = output;
                    found.push_back(foundString);
                }
            }
        } else if (state == &this->root) {
            i++;
        } else {
            state = state->failureLink;
        }
    }
    return found;
}
