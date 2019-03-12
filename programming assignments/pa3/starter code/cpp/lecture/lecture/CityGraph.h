#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include "StringGraphNode.h"
#include "Edge.h"
#include "Path.h"
#include <unordered_map>
#include <string>
#include <queue>
using namespace std;

class PairComparer
{
public:
   bool operator()(pair<StringGraphNode*, int> first, pair<StringGraphNode*, int> second)
   {
      return first.second > second.second;
   }
};

class TupleComparer
{
public:
	bool operator()(tuple<StringGraphNode*, int, vector<string>> first, tuple<StringGraphNode*, int, vector<string>> second)
	{
		return get<1>(first) > get<1>(second); 
	}
};

class CityGraph
{
private:
	unordered_map<string, StringGraphNode*> _graph;

	unordered_map<string, unordered_map<string, int>> _shortest_paths{}; 

public:
	void addVertex(const string& key)
	{
		_graph[key] = new StringGraphNode(key);
	}

	void addVertex(StringGraphNode* node)
	{
		_graph[node->getKey()] = node;
	}

	void connectVertex(
		const string& source,
		const string& sink,
		const int& weight,
		bool is_bidirectional = false)
	{
		_graph[source]->addEdge(_graph[sink], weight);
		if (is_bidirectional == true)
		{
			connectVertex(sink, source, weight, false);
		}
	}

	vector<Edge> computeMinimumSpanningTree(const string& start_key)
	{
		//where all of the edges should go
		priority_queue<Edge, vector<Edge>, Edge> mst_queue{};

		//tracks whether or not we've seen a given node in the MST
		unordered_map<string, int> discoveries{};
		discoveries[start_key] = 1;

		//constructs final end product
		vector<Edge> accepted_edges{};

		//grab starting vertex.  Construct all edges from start
		//add as primer to PQ.
		StringGraphNode* start = _graph[start_key];
		for (auto vertex : start->getEdges())
		{
			Edge e{ 
				start, 
				dynamic_cast<StringGraphNode*>(vertex.first), 
				vertex.second 
			};
			mst_queue.push(e);
		}

		//use PQ to construct MST
		while (mst_queue.empty() == false && discoveries.size() < _graph.size())
		{
			Edge top = mst_queue.top();
			mst_queue.pop();

			//have we NOT seen the top before
			if (discoveries.find(top.sink->getKey()) == discoveries.end())
			{
				//mark discovered
				discoveries[top.sink->getKey()] ++;

				//accept edge
				accepted_edges.push_back(top);

				//add in outgoing edges
				for (auto vertex : top.sink->getEdges())
				{
					Edge e{
						top.sink,
						dynamic_cast<StringGraphNode*>(vertex.first),
						vertex.second
					};
					mst_queue.push(e);
				}
			}
		}
		return accepted_edges;
	}

	unordered_map<string, int> computeShortestPath(const string& start)
	{
		//return value
		unordered_map<string, int> distances{};

		//make sure we received a valid starting point
		if (_graph.find(start) != _graph.end())
		{
			//define PQ
			priority_queue < pair<StringGraphNode*, int>,
				vector<pair<StringGraphNode*, int>>,
				PairComparer> to_visit{};

			//prime PQ with starting location
			to_visit.push(make_pair(_graph[start], 0));

			while (to_visit.empty() == false)
			{
				auto top = to_visit.top();
				string key = top.first->getKey();
				int weight = top.second;
				to_visit.pop();

				//have we seen top before?
				if (distances.find(key) == distances.end())
				{
					//mark as visited
					distances[key] = weight;

					//push all unknown outoing edges into PQ
					for (auto edge : top.first->getEdges())
					{
						StringGraphNode* node = dynamic_cast<StringGraphNode*>(edge.first);
						if (distances.find(node->getKey()) == distances.end())
						{
							to_visit.push(make_pair(node, weight + edge.second));
						}
					}
				}
			}
		}
		_shortest_paths[start] = distances; 
		return distances;
	}

	// returns true if a node with the name node_name exists in the graph
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

	// returns a HT of shortest paths to other nodes from start
	unordered_map<string, int> getShortestPaths(const string& start)
	{
		// if the shortest distance HT has already been calculated, return it
		if (_shortest_paths.find(start) != _shortest_paths.end())
		{
			return _shortest_paths[start]; 
		}
		// else, calculate it and then return it
		else
		{
			return computeShortestPath(start); 
		}
	}

	// computes paths from start onward that touch every single node in the graph
	// returns a hashtable of all paths found that meet this criteria 
	// (altered form of Dijkstra's algorithm/Prim's MST algorithm)
	unordered_map<string, Path<string>> findFullPath(const string& start)
	{
		// will return this
		unordered_map<string, Path<string>> paths{}; 

		// keeps track of how many times we've visited a node
		unordered_map<string, int> visits{}; 

		// make sure we received a valid starting point 
		if (_graph.find(start) != _graph.end())
		{
			priority_queue<tuple<StringGraphNode*, int, vector<string>>,
				vector<tuple<StringGraphNode*, int, vector<string>>>,
				TupleComparer> to_visit{}; 

			// prime the PQ with starting location
			to_visit.push(make_tuple(_graph[start], 0, vector<string>{})); 

			while (to_visit.empty() == false)
			{
				//get item on top of PQ
				auto top = to_visit.top();
				string key = get<0>(top)->getKey();
				int weight = get<1>(top);
				vector<string> path = get<2>(top);
				to_visit.pop();

				// have we already seen this node twice? 
				if (visits[key] < 2)
				{
					// if not, insert this node at the end of its path 
					path.push_back(key); 

					// mark as visited and update paths
					visits[key]++; 
					paths[key].setWeight(weight); 
					paths[key].setPath(path);

					// push ALL outgoing edges into PQ
					for (auto edge: get<0>(top)->getEdges())
					{
						// need to dynamically cast from generic graph node
						// into string graph nodes
						StringGraphNode* node = dynamic_cast<StringGraphNode*>(edge.first); 

						// has this node already been visited twice?
						if (visits[key] < 2)
						{
							// if not, push it in, accumulating distance and path
							to_visit.push(make_tuple(node, weight + edge.second, path)); 
						}
					}
				}
			}
		}
		return paths; 
	}
};

#endif // !GRAPH_H
