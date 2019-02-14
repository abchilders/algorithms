/*
Assignment: PA #1 - Huffman Coding
Description: This program uses Huffman codes to compress files.
Author: Alex Childers
HSU ID: 013145324
Completion time: 10.75 hours
In completing this program, I received help from the following people:
	N/A
*/

// This file is written by Adam Carter, with some of my own comments added. 

#ifndef HUFFMAN_LEAF_NODE
#define HUFFMAN_LEAF_NODE

// leaf nodes in Huffman trees have the values
// notice: they have no children
#include "HuffmanNode.h"
template <typename T>
class HuffmanLeafNode : public HuffmanNode<T>
{
private:
	T _value;
public:
	HuffmanLeafNode(const T &value, const int &frequency) : HuffmanNode<T>()
	{
		_value = value;
        HuffmanNode<T>::_weight = frequency;
	}

	const T &getValue()
	{
		return _value;
	}

	virtual bool isLeaf()
	{
		return true;
	}
};

#endif
