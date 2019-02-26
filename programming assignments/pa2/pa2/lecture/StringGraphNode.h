

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
