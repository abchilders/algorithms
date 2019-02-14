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

#ifndef HUFFMAN_NODE_H
#define HUFFMAN_NODE_H

// base class for HuffmanInternalNode and HuffmanLeafNode

template <typename T>
class HuffmanNode {
protected:
	int _weight;
public:
    HuffmanNode()
    {

    }

	//gets weight of huffman node and children
	virtual int getWeight()
	{
		return _weight;
	}

	//determines if node is a leaf or not
	virtual bool isLeaf() = 0;
};

#endif
