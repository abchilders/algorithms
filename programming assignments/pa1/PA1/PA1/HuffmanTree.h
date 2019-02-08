#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include "HuffmanInternalNode.h"
#include "HuffmanLeafNode.h"

// this class is similar to a BinaryNode class
template <typename T>
class HuffmanTree
{
private:
	// pointer to the root of the Huffman tree
	HuffmanNode<T> *_root;

public:
	// constructor - creates a tree with one Huffman node containing 
	// value (char) and weight (frequency)
	HuffmanTree(const T& value, const int &weight)
	{
		_root = new HuffmanLeafNode<T>(value, weight);
	}

	// returns the root node of the tree
	HuffmanNode<T> *getRoot()
	{
		return _root;
	}

	// constructor - merges two existing trees 
	HuffmanTree(HuffmanTree<T> *left, HuffmanTree<T> *right)
	{
		_root = new HuffmanInternalNode<T>(left->getRoot(), right->getRoot());
	}

	// constructor - creates tree given an already-existing node 
	HuffmanTree(HuffmanInternalNode<T> *root)
	{
		_root = root;
	}

	// destructor -- I'll trust that Adam did this correctly
	~HuffmanTree()
	{
		vector<HuffmanNode<T> *> nodes{};
		nodes.push_back(_root);
		while (nodes.size() > 0)
		{
			HuffmanNode<T> *last = nodes[nodes.size() - 1];
			nodes.pop_back();
			HuffmanInternalNode<T> *casted_node = dynamic_cast<HuffmanInternalNode<T> *>(last);

			//make sure that we're dealing with an actual
			//binary node
			if (casted_node != nullptr)
			{
				//add last's children to the stack
				nodes.push_back(casted_node->getLeftChild());
				nodes.push_back(casted_node->getRightChild());

				//having gotten all of the information out of
				//last, we can now delete the node
				delete casted_node;
			}
		}
	}

	// returns the root's weight?
	virtual int getWeight()
	{
		return _root->getWeight();
	}
};

#endif