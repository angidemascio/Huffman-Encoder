#include "huffman.h"
#include <iostream>

void HuffmanCoding::buildTree(const std::string& text) {
    std::unordered_map<char, unsigned> frequencies;
    for (char c : text) {
        frequencies[c]++;
    }

    std::priority_queue<std::shared_ptr<HuffmanNode>,
        std::vector<std::shared_ptr<HuffmanNode>>,
        CompareNodes> pq;

    for (const auto& pair : frequencies) {
        pq.push(std::make_shared<HuffmanNode>(pair.first, pair.second));
    }

    // Special case: if there's only one unique character
    if (pq.size() == 1) {
        auto node = pq.top();
        pq.pop();
        root = std::make_shared<HuffmanNode>('\0', node->frequency);
        root->left = node;
        return;
    }

    while (pq.size() > 1) {
        auto left = pq.top(); pq.pop();
        auto right = pq.top(); pq.pop();

        auto parent = std::make_shared<HuffmanNode>(left, right);
        pq.push(parent);
    }

    if (!pq.empty()) {
        root = pq.top();
    }
}

void HuffmanCoding::generateCodes(std::shared_ptr<HuffmanNode> node, const std::string& code) {
    if (!node) return;

    if (node->isLeaf()) {
        codeTable[node->character] = code;
        return;
    }

    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

std::string HuffmanCoding::encode(const std::string& text) {
    codeTable.clear();
    root = nullptr;

    buildTree(text);

    generateCodes(root, "");

    std::string encoded;
    for (char c : text) {
        encoded += codeTable[c];
    }

    return encoded;
}

std::string HuffmanCoding::decode(const std::string& encoded, const std::string& serializedTree) {
    if (!serializedTree.empty()) {
        size_t index = 0;
        root = deserializeTree(serializedTree, index);
    }

    if (!root) {
        return "";
    }

    std::string decoded;
    auto current = root;

    for (char bit : encoded) {
        if (bit == '0') {
            current = current->left;
        }
        else if (bit == '1') {
            current = current->right;
        }

        if (current->isLeaf()) {
            decoded += current->character;
            current = root;
        }
    }

    return decoded;
}

const std::unordered_map<char, std::string>& HuffmanCoding::getCodeTable() const {
    return codeTable;
}

void HuffmanCoding::serializeTree(std::shared_ptr<HuffmanNode> node, std::string& serialized) {
    if (!node) return;

    if (node->isLeaf()) {
        serialized += "1";
        serialized += node->character;
    }
    else {
        serialized += "0";
    }

    serializeTree(node->left, serialized);
    serializeTree(node->right, serialized);
}

std::string HuffmanCoding::serializeTree() {
    std::string serialized;
    serializeTree(root, serialized);
    return serialized;
}

std::shared_ptr<HuffmanNode> HuffmanCoding::deserializeTree(const std::string& serialized, size_t& index) {
    if (index >= serialized.size()) return nullptr;

    char nodeType = serialized[index++];

    if (nodeType == '1') {
        if (index < serialized.size()) {
            char c = serialized[index++];
            return std::make_shared<HuffmanNode>(c, 0);
        }
    }
    else if (nodeType == '0') {
        auto left = deserializeTree(serialized, index);
        auto right = deserializeTree(serialized, index);
        return std::make_shared<HuffmanNode>(left, right);
    }

    return nullptr;
}