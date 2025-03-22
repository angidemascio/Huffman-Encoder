#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include <memory>

struct HuffmanNode {
    char character;
    unsigned frequency;
    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;

    HuffmanNode(char ch, unsigned freq) :
        character(ch), frequency(freq), left(nullptr), right(nullptr) {}

    HuffmanNode(std::shared_ptr<HuffmanNode> l, std::shared_ptr<HuffmanNode> r) :
        character('\0'), frequency(l->frequency + r->frequency), left(l), right(r) {}

    bool isLeaf() const { return !left && !right; }
};

struct CompareNodes {
    bool operator()(const std::shared_ptr<HuffmanNode>& a, const std::shared_ptr<HuffmanNode>& b) {
        return a->frequency > b->frequency;
    }
};

class HuffmanCoding {
private:
    std::shared_ptr<HuffmanNode> root;
    std::unordered_map<char, std::string> codeTable;

    void buildTree(const std::string& text);

    void generateCodes(std::shared_ptr<HuffmanNode> node, const std::string& code);

    void serializeTree(std::shared_ptr<HuffmanNode> node, std::string& serialized);

    std::shared_ptr<HuffmanNode> deserializeTree(const std::string& serialized, size_t& index);

public:
    HuffmanCoding() : root(nullptr) {}

    std::string encode(const std::string& text);

    std::string decode(const std::string& encoded, const std::string& serializedTree);

    const std::unordered_map<char, std::string>& getCodeTable() const;

    std::string serializeTree();
};

#endif