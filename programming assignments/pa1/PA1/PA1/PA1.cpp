#include "PA1.h"

/* Remember, to construct Huffman tree: we construct frequency distribution
		 of all chars in file. (use unordered_map)
		 -Each character has a weight.
		 -Use a min-PQ to build individual trees together into a forest.
		Our responsibility: pop 2 things, merge, and push back onto PQ.

		This function is done when everything is combined in one tree.
	*/

	//Builds a Huffman Tree from the supplied vector of strings.
	//This function implement's Huffman's Algorithm as specified in the
	//book.

	//In order for your tree to be the same as mine, you must take care 
	//to do the following:
	//1.	When merging the two smallest subtrees, make sure to place the 
	//      smallest tree on the left side!

// Generates a Huffman character tree from the supplied vector of strings
HuffmanTree<char>* PA1::huffmanTreeFromText(vector<string> data)
{
	//store frequencies in hashtable
	unordered_map<char, int> frequencies{}; 

	// build character frequency distribution
	for (auto word : data)
	{
		for (auto ch : word)
		{
			frequencies[ch]++; 
		}
	}

	//maintains huffman tree forest 
					// is a queue of HuffmanTrees, will be compared with Treecomparer
    priority_queue < HuffmanTree<char>*, vector<HuffmanTree<char>*>, TreeComparer> forest{};
	for (auto kvp : frequencies)
	{
		// create a tree for the current character & its frequency; add to PQ
		forest.push(new HuffmanTree<char>(kvp.first, kvp.second)); 
	}

	// while there are still multiple forests to be merged
	while (forest.size() > 1)
	{
		// merge the top 2 trees
		// get two trees with least weight on PQ, merge them, and push back into PQ
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

//PA #1 TODO: Generates a Huffman character tree from the supplied encoding map
//NOTE: I used a recursive helper function to solve this!
HuffmanTree<char>* PA1::huffmanTreeFromMap(unordered_map<char, string> huffmanMap)
{
    //Generates a Huffman Tree based on the supplied Huffman Map.Recall that a 
    //Huffman Map contains a series of codes(e.g. 'a' = > 001).
	//Each digit(0, 1) 
    //in a given code corresponds to a left branch for 0 and right branch for 1.

	// create an initial tree
	HuffmanTree<char>* char_tree{}; 

	// build tree using code for each character
	for (auto kvp : huffmanMap)
	{

	}

    return nullptr;
}

// build a map for each char's encoding. pass by reference so that each 
// recursive call is working on the exact same map 
void huffmanEncodingMapFromTreeHelper(
	unordered_map<char, string>& map,
	HuffmanNode<char>* node,
	string encoding
)
{
	if (node->isLeaf() == false)
	{
		// not a leaf, so make recursive calls.

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
		// this IS a leaf. This means that we have a complete mapping
		// for this character. No more recursion.

		// now that we know that this node is a leaf, we need to convert node,
		// which is an HuffmanNode, into a HuffmanLeafNode so it can see its value.
		//                            convert      type that we want       what we want to convert
		HuffmanLeafNode<char>* root = dynamic_cast<HuffmanLeafNode<char>*>(node); 
		map[root->getValue()] = encoding; 
		return; 
	}
}
//PA #1 TODO: Generates a Huffman encoding map from the supplied Huffman tree
//NOTE: I used a recursive helper function to solve this!
            //char, binary code
unordered_map<char, string> PA1::huffmanEncodingMapFromTree(HuffmanTree<char> *tree)
{
	unordered_map<char, string> result{}; 
	huffmanEncodingMapFromTreeHelper(result, tree->getRoot(), ""); 
    return result;
}

//PA #1 TODO: Writes an encoding map to file.  Needed for decompression.
void PA1::writeEncodingMapToFile(unordered_map<char, string> huffmanMap, string file_name)
{
    //Writes the supplied encoding map to a file.  My map file has one 
    //association per line (e.g. 'a' and 001 would yield the line "a001")
}

//PA #1 TODO: Reads an encoding map from a file.  Needed for decompression.
unordered_map<char, string> PA1::readEncodingMapFromFile(string file_name)
{
    //Creates a Huffman Map from the supplied file.Essentially, this is the 
    //inverse of writeEncodingMapToFile.  
    unordered_map<char, string> result{};
    return result;
}

//PA #1 TODO: Converts a vector of bits (bool) back into readable text using the supplied Huffman map
string PA1::decodeBits(vector<bool> bits, unordered_map<char, string> huffmanMap)
{
    //Uses the supplied Huffman Map to convert the vector of bools (bits) back into text.
    //To solve this problem, I converted the Huffman Map into a Huffman Tree and used 
    //tree traversals to convert the bits back into text.

	// write to string: result << "abc" << endl; 
	// we do this because strings are a fixed size - constantly creating new 
	// string and reassigning is wasteful. doing this saves time - creates 
	// buffer and tacks characters onto the end 
    ostringstream result{};

	// after you have the tree, walk down the tree until you see a leaf node 
	/* 
	if leaf node:
	result << node->value
	*/

	// converts from ostringstream into actual string
    return result.str();
}

//PA #1 TODO: Using the supplied Huffman map compression, converts the supplied text into a series of bits (boolean values)
vector<bool> PA1::toBinary(vector<string> text, unordered_map<char, string> huffmanMap)
{
    vector<bool> result{};
    return result;
}