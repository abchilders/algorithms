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

#pragma once
#ifndef EDGE_H
#define EDGE_H

#include "StringGraphNode.h"

class Edge
{
public:
	StringGraphNode* source = nullptr;
	StringGraphNode* sink = nullptr;
	int weight = 0;

	Edge(StringGraphNode* source = nullptr, StringGraphNode* sink = nullptr,
		int weight = 0)
	{
		this->sink = sink;
		this->source = source;
		this->weight = weight;
	}

	bool operator()(const Edge& lhs, const Edge& rhs)
	{
		return lhs.weight > rhs.weight;
	}
};


#endif