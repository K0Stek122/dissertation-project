#include <map>
#include <vector>
#include <set>

class Node {
private:
public:
    std::map<char, Node*> children;
    Node* failureLink = nullptr;
    std::set<Node> outputs;
    
private:

public:
    bool hasChild(const char& key);
    Node* getChild(const char& key);
    void setChild(const char &key, Node* node);
    void addOutput(const Node& output);
    void copyOutputs(const Node& node);
    
    bool operator<(const Node& other) const;
};