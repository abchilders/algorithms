/*
Assignment: PA #2 - Dijkstra's algorithm
Description: This program will use CSV information on campus walking times
	to prompt the user for two campus location codes and calculate the
	estimated shortest travel time using Dijkstra's algorithm, as well as
	outputting what buildings one will pass on the way.
Author: Alex Childers
HSU ID: 013145324
Completion time: 12.75 hours
In completing this program, I received help from the following people:
	N/A
*/

// Provided by Adam Carter, comments by Alex Childers. 

#ifndef STRING_GRAPH_NODE_H
#define STRING_GRAPH_NODE_H

#include "GraphNode.h"
#include <string>
using namespace std;

// Abstraction of GraphNode, because CampusGraph deals with strings.
class StringGraphNode : public GraphNode<string, string>
{
public:
   StringGraphNode(const string &k) : GraphNode(k, k)
   {

   };
};

#endif // !STRING_GRAPH_NODE_H
