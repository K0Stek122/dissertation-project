#include <map>
#include <vector>
#include <set>
#include <string>
#include <queue>

struct FoundString{
    bool operator==(const FoundString& rhs) const;
    int pos;
    std::string val;
};

class Node {
private:
public:
    std::map<char, Node*> children;
    Node* failureLink = nullptr;
    std::set<std::string> outputs;
    
private:

public:
    bool hasChild(const char& key);
    Node* getChild(const char& key);
    void setChild(const char &key, Node* node);
    void addOutput(const std::string& output);
    void copyOutputs(const Node& node);
    
    bool operator<(const Node& other) const;
};

class AhoCorasick {
public:
    Node root;

public:
    AhoCorasick(std::vector<std::string> patterns);
    
    std::vector<FoundString> search(const std::string& text);
};