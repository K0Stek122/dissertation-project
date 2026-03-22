class Node:
    def __init__(self):
        self.children = {}
        self.failureLink = None
        self.outputs = set()

    def hasChild(self, key):
        if key in self.children:
            return True
        else:
            return False

    def getChild(self, key):
        return self.children.get(key)

    def setChild(self, key, node):
        self.children[key] = node

    def addOutput(self, output):
        self.outputs.add(output)

    def copyOutputs(self, node : Node):
        for o in node.outputs:
            self.outputs.add(o)

class AhoCorasick:
    def __init__(self, patterns):
        # Construct the trie
        self.root = Node();
        currNode = self.root
        for pattern in patterns:
            for char in pattern:
                key = char
                if not currNode.hasChild(key):
                    currNode.setChild(key, Node())
                currNode = currNode.getChild(key)
            currNode.addOutput(pattern)
            currNode = self.root

        # Failure link
        self.root.failureLink = self.root

        queue = []
        for child in self.root.children.values():
            child.failureLink = self.root
            queue.append(child)

        while len(queue) != 0:
            currNode = queue.pop(0)
            for key, child in currNode.children.items():
                queue.append(child)
                n = currNode.failureLink
                while n != self.root and not n.hasChild(key):
                    n = n.failureLink
                child.failureLink = n.getChild(key) if n.getChild(key) else self.root
                child.copyOutputs(child.failureLink)

    def search(self, text):
        found = []
        state = self.root
        i = 0
        while i < len(text):
            c = text[i]
            if state.hasChild(c):
                state = state.getChild(c)
                i = i + 1
                if len(state.outputs) > 0:
                    for output in state.outputs:
                        found.append({"pos" : i - len(output), "val" : output})
            elif state == self.root:
                i = i + 1
            else:
                state = state.failureLink
        return found

machine = AhoCorasick(["eat", "eating", "meat", "in"])
result = machine.search("I am eating meat")
for item in result:
    for key, value in item.items():
        print(f"{key} : {value}")
