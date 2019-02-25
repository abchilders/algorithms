// For 2nd tier: you need to know not just node and weight,
// but entire path.
// Could push on node into a member variable called vector, and then weight = 
// sum of all nodes. 
// Adam made a separate path class to contain the path. 

#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include "StringGraphNode.h"
#include <unordered_map>
#include <string>
#include <queue>
using namespace std;

// may have to rewrite this for Tier 2
class PairComparer
{
public:				// what PQ operates on
   bool operator()(pair<StringGraphNode*, int> first, pair<StringGraphNode*, int> second)
   {
      return first.second > second.second;
   }
};

// manages nodes in campus graph
// will represent graph with unordered_map
class CampusGraph
{
private:
	// represents our full campus graph
			// node name, pointer to graph node
	unordered_map<string, StringGraphNode*> _graph;

	// holds abbreviated and full names of buildings on campus, and which 
	// node they belong to 
				// abbr.      <full building name, what node it belongs to>
	unordered_map<string, pair<string, string>> _building_codes;

public:
	void addVertex(const string& key)
	{
		// adds an item to our graph
		_graph[key] = new StringGraphNode(key);
	}

	// passes in an already-existing node directly
	void addVertex(StringGraphNode* node)
	{
		_graph[node->getKey()] = node;
	}

	// connect two nodes
	void connectVertex(
		const string& source,
		const string& sink,
		const int& weight,
		bool is_bidirectional = false) // bidirectional is false by default
	{
		_graph[source]->addEdge(_graph[sink], weight);
		if (is_bidirectional == true)
		{
			// connect it the other way round 
			connectVertex(sink, source, weight, false);
		}
	}

	// TO DO: implement pushing nodes onto each node's private path variable

	// Dijkstra's shortest path algorithm.
	// See lecture notes for visualization.
	// destination, time it takes to get there		// where we start on map
	unordered_map<string, int> computeShortestPath(const string& start)
	{
		// will return this 
		unordered_map<string, int> distances{};

		// make sure we received a valid starting point
		if (_graph.find(start) != _graph.end()) // if we found the starting point 
		{
			// pointer, distance
			priority_queue<pair<StringGraphNode*, int>, // what's in the PQ
				vector<pair<StringGraphNode*, int>>, // how we store these pairs
				PairComparer> to_visit{};  // how we compare each pair

			// prime the PQ with starting location
			to_visit.push(make_pair(_graph[start], 0));
			//_graph[start]->addPathNode(_graph[start], _graph[start]); 

			while (to_visit.empty() == false)
			{
				// get item on top of PQ
				auto top = to_visit.top();
				string key = top.first->getKey();
				int weight = top.second;
				to_visit.pop();

				// have we seen this node yet, in the distances{} map? 
				// first = node, which has key
				if (distances.find(key) == distances.end())
				{
					// if we get to the end, we have NOT seen the key
					// so mark as visited in our distances map
					distances[key] = weight;

					// the shortest path to key has been found, so insert it 
					// at the end of its own path
					top.first->addPathNode(_graph[start], top.first); 

					// push all unknown outgoing edges into PQ
					for (auto edge : top.first->getEdges())
					{
						// need to dynamically cast from generic graph node 
						// into string graph nodes
						StringGraphNode* node = dynamic_cast<StringGraphNode*>(edge.first);

						// is that node already in distances?
						if (distances.find(node->getKey()) == distances.end())
						{
							// if not, add its "parent node" to its path 
							// and push it in, accumulating distance
							node->addPathNode(_graph[start], top.first); 
							to_visit.push(make_pair(node, weight + edge.second));
						}
					}
				}
			}

		}
		return distances;
	}

	// Returns true if a node with the name node_name exists in the graph.
	bool nodeExists(string node_name)
	{
		if (_graph[node_name] == nullptr)
		{
			return false; 
		}
		else
		{
			return true; 
		}
	}

	// adds a building to the _building_codes hashtable
	void addBuilding(string abbr, string full_name, string parent_node)
	{
		pair<string, string> building_info{ full_name, parent_node }; 
		_building_codes[abbr] = building_info; 
	}

	// returns the full name of a building given by its abbreviation
	string getBuildingName(string abbr)
	{
		return _building_codes[abbr].first; 
	}

	// returns the name of the node that a building belongs to
	string getBuildingNodeName(string abbr)
	{
		return _building_codes[abbr].second; 
	}

	// returns a pointer to the node that a building belongs to
	StringGraphNode* getBuildingNode(string abbr)
	{
		return _graph[_building_codes[abbr].second]; 
	}
};

#endif // !GRAPH_H
