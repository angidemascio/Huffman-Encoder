# Huffman Encoder

This project is a simple implementation of Huffman Coding for text compression and decompression that works through a command-line interface.

## How It Works
1. The process starts with analyzing the frequencies of characters in the input data. 
2. A priority queue is then made to construct the binary tree. 
3. Once the tree is built, each character is assigned a unique binary sequence based on the tree's structure. 
4. The encoded binary data and the serialized Huffman tree are stored in an efficient manner. 
5. To decompress, the tree is reconstructed and the binary data is translated back into the original content.

## Usage
### Compression
```sh
./huffman compress input.txt compressed.huff
```
This compresses `input.txt` and saves the output as `compressed.huff`.

### Decompression
```sh
./huffman decompress compressed.huff output.txt
```
This restores the original text in `output.txt`.
