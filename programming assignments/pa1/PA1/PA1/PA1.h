/*
Assignment: PA #1 - Huffman Coding
Description: This program uses Huffman codes to compress files.
Author: Alex Childers
HSU ID: 013145324
Completion time: 8.75 hours
In completing this program, I received help from the following people:
	N/A
*/

// This file is written by Adam Carter, with some of my own comments added.

#ifndef PA_1_H
#define PA_1_H

#include "HuffmanTree.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <queue>
#include <unordered_map>
#include "BinaryFile.h"
#include "StringSplitter.h"


using namespace std;

// used to compare trees based on weight
class TreeComparer
{
public:
    bool operator()(HuffmanTree<char> * first, HuffmanTree<char> * second)
    {
        return first->getWeight() > second->getWeight();
    }
};

class PA1
{
public:

	// Generates a Huffman character tree from the supplied text
	static HuffmanTree<char> *huffmanTreeFromText(vector<string> data);

	// Generates a Huffman character tree from the supplied encoding map
	static HuffmanTree<char> *huffmanTreeFromMap(unordered_map<char, string> huffmanMap);

	// Generates a Huffman encoding map from the supplied Huffman tree
	static unordered_map<char, string> huffmanEncodingMapFromTree(HuffmanTree<char> *tree);

	// Writes an encoding map to file.  Needed for decompression.
	static void writeEncodingMapToFile(unordered_map<char, string> huffmanMap, string file_name);

	// Reads an encoding map from a file.  Needed for decompression.
	static unordered_map<char, string> readEncodingMapFromFile(string file_name);

	// Converts a vector of bits (bool) back into readable text using the supplied Huffman map
	static string decodeBits(vector<bool> bits, unordered_map<char, string> huffmanMap);

	// Using the supplied Huffman map compression, converts the supplied text into a series of bits (boolean values)
	static vector<bool> toBinary(vector<string> text, unordered_map<char, string> huffmanMap);
};

#endif
