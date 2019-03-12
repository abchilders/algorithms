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

#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include <unordered_map>
#include <vector>
using namespace std;

template <typename Key, typename Value>
class GraphNode
{
protected:

    //what's inside of our node
    Value _value;

    //how we refer to this node
    Key _key;

    //Key to HT is our key type
    //Value of HT is another node in our graph
    unordered_map<GraphNode<Key, Value>*, int> _vertices;

public:

    GraphNode(const Key &k, const Value &v)
    {
        _value = v;
        _key = k;
    }

    GraphNode()
    {

    }

    //allows for polymorphic behavior
    virtual ~GraphNode() {}

    Value &getValue()
    {
        return _value;
    }

    void setValue(const Value& v)
    {
        _value = v;
    }

    Key& getKey()
    {
        return _key;
    }

    const Key& getKey() const
    {
        return _key;
    }

    void setKey(const Key& k)
    {
        _key = k;
    }

    void addEdge(GraphNode<Key, Value> *node, int weight = 1)
    {
        _vertices[node] = weight;
    }

    unordered_map<GraphNode<Key, Value>*, int> getEdges()
    {
       return _vertices;
    }

};

template <typename Key, typename Value>
bool operator<(const GraphNode<Key, Value> &left,
               const GraphNode<Key, Value> &right
    )
{
    return left.getKey() < right.getKey();
}

template <typename Key, typename Value>
bool operator>(const GraphNode<Key, Value> &left,
    const GraphNode<Key, Value> &right
    )
{
    return left.getKey() > right.getKey();
}

template <typename Key, typename Value>
bool operator==(const GraphNode<Key, Value> &left,
    const GraphNode<Key, Value> &right
    )
{
    return left.getKey() == right.getKey();
}

template <typename Key, typename Value>
class GraphNodeComparer
{
public:
    bool operator() (GraphNode<Key, Value>* left, 
                    GraphNode<Key, Value>* right
        )
    {
        return left > right;
    }
};

#endif // !GRAPH_NODE_H
