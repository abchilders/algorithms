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

#ifndef HUFFMAN_INTERNAL_NODE
#define HUFFMAN_INTERNAL_NODE
#include "HuffmanNode.h"
#include <vector>
using namespace std;

//internal nodes in huffman trees don't have values, they just point to other internal nodes 
//or leaf nodes

// all internal nodes are direction markers to data
template <typename T>
class HuffmanInternalNode : public HuffmanNode<T>
{
private:
	HuffmanNode<T> *_left_child = nullptr;
	HuffmanNode<T> *_right_child = nullptr;

public:
	// constructor - given two children, it assigns the children to this internal
	// node and sets this node's weight = the sum of its children's weights
    HuffmanInternalNode(HuffmanNode<T> *left_child, HuffmanNode<T> *right_child) : HuffmanNode<T>()
	{
		if (left_child != nullptr && right_child != nullptr)
		{
            HuffmanNode<T>::_weight = left_child->getWeight() + right_child->getWeight();
		}
		_left_child = left_child;
		_right_child = right_child;
	}

	//internal nodes are not leaf nodes!
	virtual bool isLeaf()
	{
		return false;
	}

	HuffmanNode<T> *getLeftChild() const
	{
		return _left_child;
	}

	HuffmanNode<T> *getRightChild() const
	{
		return _right_child;
	}

	void setLeftChild(HuffmanNode<T> *node)
	{
		_left_child = node;
	}

	void setRightChild(HuffmanNode<T> *node)
	{
		_right_child = node;
	}
};

#endif