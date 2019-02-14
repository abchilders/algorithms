/*
Assignment: PA #1 - Huffman Coding
Description: This program uses Huffman codes to compress files.
Author: Alex Childers
HSU ID: 013145324
Completion time: 10.75 hours
In completing this program, I received help from the following people:
	N/A
*/

// Function headers written by Adam Carter.
// Most code content written by Alex, except for what was live-coded during class. 

#include "PA1.h"

// Builds a Huffman character tree from the supplied vector of strings.
// This function implements Huffman's Algorithm as specified in the book.
// Complexity: O(N), because we have O(N + N + N) ==> O(3N) ==> O(N)
HuffmanTree<char>* PA1::huffmanTreeFromText(vector<string> data)
{
	//store frequencies in hashtable
	unordered_map<char, int> frequencies{}; 

	// build character frequency distribution
	// Complexity: O(N) where N = # of characters in data
	for (auto word : data)
	{
		for (auto ch : word)
		{
			frequencies[ch]++; 
		}
	}

	// check for nullness at this point
	if (frequencies.size() == 0)
	{
		return nullptr; 
	}

	//maintains huffman tree forest 
					// is a queue of HuffmanTrees, will be compared with Treecomparer
    priority_queue < HuffmanTree<char>*, vector<HuffmanTree<char>*>, TreeComparer> forest{};
	
	// Complexity: O(N) 
	for (auto kvp : frequencies)
	{
		// create a tree for the current character & its frequency; add to PQ
		forest.push(new HuffmanTree<char>(kvp.first, kvp.second)); 
	}

	// for n characters, we perform n-1 merge operations
	// complexity: O(N-1) --> O(N)
	for (int i = 0; i < frequencies.size() - 1; i++)
	{
		// merge the top 2 trees and push back into PQ
		// smaller tree always goes on the left
		HuffmanTree<char>* smaller = forest.top();
		forest.pop();
		HuffmanTree<char>* larger = forest.top();
		forest.pop();
		forest.push(new HuffmanTree<char>{ smaller, larger });
	}

	// return final tree
	return forest.top();
}

// Generates a Huffman character tree from the supplied encoding map
// Helper function for decodeBits().
// Complexity: O(N^2), due to one loop nested inside another, both O(N) --> O(N * N) = O(N^2)
HuffmanTree<char>* PA1::huffmanTreeFromMap(unordered_map<char, string> huffmanMap)
{
	// start an initial tree
	HuffmanInternalNode<char>* root = new HuffmanInternalNode<char>{ nullptr, nullptr };
	HuffmanTree<char>* tree = new HuffmanTree<char>{ root }; 

	// Build the tree using the encoding for each character.
	// Complexity: O(N), where N = # of characters, * O(N) inner loop = O(N^2)
	for (auto kvp : huffmanMap)
	{
		// start our walk at the top of the tree
		HuffmanInternalNode<char>* current = root; 

		char value = kvp.first; 
		string path = kvp.second; 
		
		// walk down the tree on the given path, creating internal nodes as needed
		// Complexity: path length could be up to N - 1, so O(N-1-1) --> O(N)
		for (int i = 0; i < path.length() - 1; i++)
		{
			// represents whether we go to the left (0) or right (1) next
			char side = path[i]; 
			if (side == '0')
			{
				// does the current node need an internal node to be created on
				// its left side before we walk down?
				if (current->getLeftChild() == nullptr)
				{
					current->setLeftChild(new HuffmanInternalNode<char>{ nullptr, nullptr });
				}
				current = dynamic_cast<HuffmanInternalNode<char>*>(current->getLeftChild()); 
			}
			else
			{
				// does the current node need an internal node to be created on its
				// right side before we walk down?
				if (current->getRightChild() == nullptr)
				{
					current->setRightChild(new HuffmanInternalNode<char>{ nullptr, nullptr }); 
				}
				current = dynamic_cast<HuffmanInternalNode<char>*>(current->getRightChild());
			}
		}

		// now that we've walked down the entire path except the last step, we 
		// create a leaf node for the current character
		char last_side = path[path.length() - 1]; 
		if (last_side == '0')
		{
			// we create a leaf node with the given value and a dummy weight of 0
			current->setLeftChild(new HuffmanLeafNode<char>{ value, 0 });  
		}
		else
		{
			// same thing as above, but on the right side
			current->setRightChild(new HuffmanLeafNode<char>{ value, 0 }); 
		}
	}

    return tree;
}

// Build an encoding map from the given Huffman tree throughout 
// this function's recursive calls.
// Complexity: O(N), where N = # of characters, due to the amount of recursion needed. 
//		See below. 
void huffmanEncodingMapFromTreeHelper(
	unordered_map<char, string>& map,
	HuffmanNode<char>* node,
	string encoding
)
{
	if (node->isLeaf() == false)
	{
		// not a leaf, so make recursive calls.
		// Complexity: O(N). Since the tree is full, we must make 2 recursive calls
		// for each non-leaf node given. And, since for N leaf nodes/characters,
		// there are N-1 internal nodes, we will perform this if loop
		// 2(N-1) times ==> 2N-2 ==> N. 

		// convert node into an internal node
		HuffmanInternalNode<char>* root =
			dynamic_cast<HuffmanInternalNode<char>*>(node);

		// recursively call ourselves on our children
		huffmanEncodingMapFromTreeHelper(
			map,
			root->getLeftChild(),
			encoding + "0"
		);
		huffmanEncodingMapFromTreeHelper(
			map,
			root->getRightChild(),
			encoding + "1"
		); 
		return; 
	}
	else
	{
		// This IS a leaf. This means that we have a complete mapping
		// for this character. No more recursion.
		// complexity: O(1)

		// Now that we know that this node is a leaf, we need to convert node,
		// which is a HuffmanNode, into a HuffmanLeafNode so it can see its value.
		//                            convert      type that we want       what we want to convert
		HuffmanLeafNode<char>* root = dynamic_cast<HuffmanLeafNode<char>*>(node); 
		map[root->getValue()] = encoding; 
		return; 
	}
}
// Generates a Huffman encoding map from the supplied Huffman tree.
// Complexity: O(N), because we need O(1) for setting up the hashtable and O(N) for the call to 
//		huffmanEncodingMapFromTreeHelper(). 
unordered_map<char, string> PA1::huffmanEncodingMapFromTree(HuffmanTree<char> *tree)
{
	unordered_map<char, string> result{}; 
	if (tree != nullptr)
	{
		huffmanEncodingMapFromTreeHelper(result, tree->getRoot(), "");
	}
    return result;
}

// Writes the supplied encoding map to file.  Needed for decompression.
// Complexity: O(N), because we process the entirety of huffmanMap
void PA1::writeEncodingMapToFile(unordered_map<char, string> huffmanMap, string file_name)
{
    // The map file has one association per line. 
	// (e.g. 'a' and 001 would yield the line "a001")
	ofstream out_stream{ file_name }; 
	if (out_stream.is_open())
	{
		// O(N), where N = # of characters (size of HT)
		for (auto kvp : huffmanMap)
		{
			out_stream << kvp.first << kvp.second << endl; 
		}
	}
	out_stream.close(); 
}

// Creates a Huffman encoding map from a file.  Needed for decompression.
// Complexity: O(N), because we touch each line in file_name twice --> O(2N) ==> O(N) 
unordered_map<char, string> PA1::readEncodingMapFromFile(string file_name)
{
    unordered_map<char, string> result{};
	vector<string> lines{}; 
	string next_line = ""; 
	ifstream in_stream{ file_name }; 

	// put file contents into a string
	if (in_stream.is_open())
	{
		// O(N) -- getting and pushing back each line in file_name
		while (in_stream.good() == true)
		{
			getline(in_stream, next_line);
			if (next_line.length() > 0)
			{
				lines.push_back(next_line);
			}
		}
	}
	in_stream.close(); 

	// now iterate through the vector of character + encoding and load contents into a hashtable
	// O(N), because we're iterating through each line from file_name again so we can load 
	//		it into an HT
	for (auto char_info : lines)
	{
		//     character       encoding 
		result[char_info[0]] = char_info.substr(1, char_info.length() - 1); 
	}

    return result;
}

// Converts a vector of bits (bool) back into readable text using the supplied Huffman map
// Complexity: O(N^2), because the huffmanTreeFromMap() operation polynomically dominates 
// how much time it takes to do everything else. O(N^2) + O(M)/O(N) (however you interpret it)
// ==> something like O(N^2). 
string PA1::decodeBits(vector<bool> bits, unordered_map<char, string> huffmanMap)
{
	// convert huffmanMap to a HuffmanTree
	// reminder: O(N^2), where N = # chars
	HuffmanTree<char>* tree = huffmanTreeFromMap(huffmanMap); 

	// we'll write to a string using a buffer, to save time
    ostringstream result{};

	// now that we have the tree, walk down the tree until we see a leaf node
	// Complexity: O(M), where M = # bits
	//		(which is something like N1*freq1 + N2*freq2 + ... Nx*freqx, where each of
	//		1, 2, ... x refers to one of the unique characters in the Huffman Map, 
	//		N1, N2, ... Nx refers to the length of that character's encoding, and 
	//		freq1, freq2, ... freqx is a constant describing how often that character occurs in the text
	//		given by the binary sequence.
	//		So I think this boils down to ~O(N)? )
	HuffmanNode<char>* current = tree->getRoot(); 
	for (auto bit : bits)
	{
		// move current in the given direction: O(1)
		if (bit == 0)
		{
			current = dynamic_cast<HuffmanInternalNode<char>*>(current)->getLeftChild();
		}
		else
		{
			current = dynamic_cast<HuffmanInternalNode<char>*>(current)->getRightChild();
		}

		// if current is a leaf node
		if (current->isLeaf() == true)
		{
			// write value to string
			HuffmanLeafNode<char>* leaf = 
				dynamic_cast<HuffmanLeafNode<char>*>(current); 
			result << leaf->getValue(); 

			// reset current to root
			current = tree->getRoot(); 
		}
	}

	// converts from ostringstream into actual string
    return result.str();
}

// Using the supplied Huffman map compression, converts the supplied text 
// into a series of bits (boolean values)
// Complexity: O(M * N), where N = # chars in text and 
//		M = length of character encoding
vector<bool> PA1::toBinary(vector<string> text, unordered_map<char, string> huffmanMap)
{
    vector<bool> result{};

	// for each string in the vector
	for (auto word : text)
	{
		// for each character in the string: O(N) where N = # chars
		for (auto ch : word)
		{
			// translate the character's encoding into binary
			// O(M) where M = # bits per character encoding, up to N
			for (auto bit : huffmanMap[ch])
			{
				if (bit == '0')
				{
					result.push_back(0); 
				}
				else
				{
					result.push_back(1); 
				}
			}
		}
	}
	
    return result;
}