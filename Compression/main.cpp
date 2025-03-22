#include <iostream>
#include <string>
#include <fstream>
#include "huffman.h"

void compressFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile) {
        std::cerr << "Cannot open input file!" << std::endl;
        return;
    }

    std::string content((std::istreambuf_iterator<char>(inFile)),
        std::istreambuf_iterator<char>());
    inFile.close();

    HuffmanCoding huffman;
    std::string encoded = huffman.encode(content);
    std::string serializedTree = huffman.serializeTree();

    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile) {
        std::cerr << "Cannot open output file!" << std::endl;
        return;
    }

    size_t treeSize = serializedTree.size();
    outFile.write(reinterpret_cast<const char*>(&treeSize), sizeof(treeSize));
    outFile.write(serializedTree.c_str(), treeSize);

    std::vector<unsigned char> compressedData;
    for (size_t i = 0; i < encoded.size(); i += 8) {
        unsigned char byte = 0;
        for (int j = 0; j < 8 && i + j < encoded.size(); j++) {
            if (encoded[i + j] == '1') {
                byte |= (1 << (7 - j));
            }
        }
        compressedData.push_back(byte);
    }

    size_t dataSize = compressedData.size();
    size_t originalBits = encoded.size();
    outFile.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
    outFile.write(reinterpret_cast<const char*>(&originalBits), sizeof(originalBits));
    outFile.write(reinterpret_cast<const char*>(compressedData.data()), dataSize);

    outFile.close();

    double compressionRatio = static_cast<double>(content.size()) /
        (sizeof(treeSize) + treeSize + sizeof(dataSize) +
            sizeof(originalBits) + dataSize);

    std::cout << "Compression complete.\n";
    std::cout << "Original size: " << content.size() << " bytes\n";
    std::cout << "Compressed size: " << (sizeof(treeSize) + treeSize + sizeof(dataSize) +
        sizeof(originalBits) + dataSize) << " bytes\n";
    std::cout << "Compression ratio: " << compressionRatio << ":1" << std::endl;
}

void decompressFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile) {
        std::cerr << "Cannot open input file!" << std::endl;
        return;
    }

    size_t treeSize;
    inFile.read(reinterpret_cast<char*>(&treeSize), sizeof(treeSize));

    std::string serializedTree(treeSize, '\0');
    inFile.read(&serializedTree[0], treeSize);

    size_t dataSize, originalBits;
    inFile.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
    inFile.read(reinterpret_cast<char*>(&originalBits), sizeof(originalBits));

    std::vector<unsigned char> compressedData(dataSize);
    inFile.read(reinterpret_cast<char*>(compressedData.data()), dataSize);
    inFile.close();

    std::string encodedBits;
    for (size_t i = 0; i < dataSize; i++) {
        for (int j = 7; j >= 0; j--) {
            if (encodedBits.size() < originalBits) {
                encodedBits += ((compressedData[i] >> j) & 1) ? '1' : '0';
            }
        }
    }

    HuffmanCoding huffman;
    std::string decoded = huffman.decode(encodedBits, serializedTree);

    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile) {
        std::cerr << "Cannot open output file!" << std::endl;
        return;
    }

    outFile.write(decoded.c_str(), decoded.size());
    outFile.close();

    std::cout << "Decompression complete." << std::endl;
}

void printCodeTable(const HuffmanCoding& huffman) {
    std::cout << "Huffman Code Table:" << std::endl;
    for (const auto& pair : huffman.getCodeTable()) {
        std::cout << "'" << (pair.first == '\n' ? "\\n" :
            (pair.first == '\t' ? "\\t" :
                (pair.first == '\r' ? "\\r" :
                    std::string(1, pair.first))));
        std::cout << "' -> " << pair.second << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "Usage: \n";
        std::cout << "  For compression: " << argv[0] << " compress input_file output_file\n";
        std::cout << "  For decompression: " << argv[0] << " decompress input_file output_file\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];

    if (mode == "compress") {
        compressFile(inputFile, outputFile);
    }
    else if (mode == "decompress") {
        decompressFile(inputFile, outputFile);
    }
    else {
        std::cout << "Invalid mode. Use 'compress' or 'decompress'." << std::endl;
        return 1;
    }

    return 0;
}