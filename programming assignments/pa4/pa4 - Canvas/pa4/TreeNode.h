/*
Assignment: PA #4 - Decision Trees
Description: This program builds a decision tree based on a supplied CSV file.
Author: Alex Childers
HSU ID: 013145324
Completion time: 14 hours
In completing this program, I received help from the following people:
	N/A
*/

// This file is provided by Adam Carter. 

#pragma once
#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <unordered_map>
#include <string>

using namespace std; 

class TreeNode
{
public:
	unordered_map<string, TreeNode*> children; 
	string value; 
};

#endif // !TREE_NODE_H