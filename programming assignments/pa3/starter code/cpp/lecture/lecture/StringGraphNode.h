/*
Assignment: PA #3 - Minimum Spanning Tree
Description: Given a file containing a map of the city (where each line contains a
	source, sink, and weight all representing an edge), and a file containing
	a list of houses that will receive deliveries for the next day, this program
	uses a minimum spanning tree to compute the mimimum amount of time required
	to deliver packages to all requested houses.
Author: Alex Childers
HSU ID: 013145324
Completion time: 10 hours
In completing this program, I received help from the following people:
	N/A
*/

// This file is provided by Adam Carter. 

#ifndef STRING_GRAPH_NODE_H
#define STRING_GRAPH_NODE_H

#include "GraphNode.h"
#include <string>
using namespace std;

class StringGraphNode : public GraphNode<string, string>
{
public:
   StringGraphNode(const string &k) : GraphNode(k, k)
   {

   };
};

#endif // !STRING_GRAPH_NODE_H
