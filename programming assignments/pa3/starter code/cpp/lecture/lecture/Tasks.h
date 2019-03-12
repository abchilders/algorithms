#ifndef TASKS_H
#define TASKS_H

#include "CityGraph.h"
#include "CsvParser.h"
#include <iostream>

/* Assignment tasks:
- load in and create the graph 
- construct an MST on that graph
- output the total cost of that route
	- ASK: "route plus the accepted edges"-- what does this mean? 
*/

class Tasks
{
private:
	// contains our overall graph 
	CityGraph _city_graph{}; 

	// will contain a reduced graph based on the Tier 2 MST
	CityGraph _reduced_graph{}; 

	// contains a list of the nodes we want to deliver to- the int will
	// default to 1 to indicate that a node exists in this list (will be
	// useful when reducing the graph for Tier 2) 
	unordered_map<string, int> _deliveries{}; 

public: 
	// interfaces with user to get user input and output results 
	void userInterface()
	{
		// get user input
		string city_map_file = "";
		string dest_file = "";
		cout << "***Route Planner***" << endl;
		cout << "Enter maps file: ";
		getline(cin, city_map_file);
		cout << "Enter destinations file: ";
		getline(cin, dest_file);

		// create full city graph 
		createFullGraph(city_map_file);

		// record deliveries
		parseDeliveries(dest_file); 

		// Tier 2: reduce the graph using Dijkstra's algorithm, then create an MST 
		// on that reduced graph 
		vector<Edge> reduced_map = createReducedMst(); 

		// Tier 3: calculate a route on our reduced graph 
		string path = calculateMstRoute(reduced_map); 

		// Tier 2/3: calculate the total transit time on this MST
		int transit_time = 0; 
		for (auto edge : reduced_map)
		{
			transit_time += edge.weight; 
		}

		// for all nodes involved in 3+ edges, calculate the paths to each dead
		// end. stop calculating if you run into another node with 3+ edges along
		// the way; that node can calculate the distances to the rest of the dead 
		// ends
		
		// Tier 2: output results
		cout << "Total transit time: " << transit_time << " minutes" << endl;
	}

	// uses a CSV map file to create a CityGraph
	void createFullGraph(string map_file)
	{
		// parse CSV map_file
		CsvStateMachine map_csm{ map_file };
		vector<vector<string>> map_data = map_csm.processFile();

		// for each edge in the given map data
		for (auto edge_info : map_data)
		{
			string first_house = edge_info[0];
			string second_house = edge_info[1];
			int weight = stoi(edge_info[2]);

			// add this edge to the graph 
			addEdge(first_house, second_house, weight, _city_graph); 
		}
	}

	// adds an edge to the graph 
	void addEdge(string source, string sink, int weight, CityGraph& graph)
	{
		// add the houses to the graph if they don't already exist
		if (graph.nodeExists(source) == false)
		{
			graph.addVertex(source);
		}
		if (graph.nodeExists(sink) == false)
		{
			graph.addVertex(sink);
		}

		// connect bidirectionally 
		graph.connectVertex(source, sink, weight, true);
	}

	// reads a list of houses to deliver to from a file
	void parseDeliveries(string delivery_file)
	{
		ifstream dest_stream{delivery_file}; 
		string next_house = ""; 

		// read in deliveries
		if (dest_stream.is_open() == true)
		{
			while (dest_stream.good() == true)
			{
				dest_stream >> next_house;
				_deliveries[next_house] = 1; 
			}
		}
		dest_stream.close(); 
		
	}

	// reduces _city_graph so that it only contains the vertices in 
	// _deliveries, with edge weights derived from Dijkstra's algorithm.
	// returns the MST of this reduced graph.
	vector<Edge> createReducedMst()
	{
		unordered_map<string, unordered_map<string, int>> shortest_paths{}; 

		// compute shortest paths on each destination node, creating our reduced
		// graph as we go
		for (auto house : _deliveries)
		{
			// i didn't want to use unordered_map::find because of the potentially linear
			// runtime, so this is my attempt to check if something exists in the HT
			if (house.second == 1)
			{
				string start = house.first;
				shortest_paths[start] = _city_graph.computeShortestPath(house.first);

				// for each sink that we've found a shortest path to on the current house
				for (auto dest : shortest_paths[start])
				{
					string end = dest.first;

					// if the end/sink is one of our destination nodes
					if (_deliveries[end] == 1)
					{
						// add it to the graph
						addEdge(start, end, shortest_paths[start][end], _reduced_graph);
					}
				}
			}
		}

		// build an MST on this graph and return the result
		return _reduced_graph.computeMinimumSpanningTree(shortest_paths.begin()->first);
	}

	string calculateMstRoute(vector<Edge> mst)
	{
		// assumption: the MST has at least two nodes that are involved in exactly
		// one edge (because if there were less than two nodes at "dead ends" in the 
		// route, that implies that the tree is NOT a truly "minimum" spanning tree

		// how many times is each node involved in an edge? (if there is only one 
		// possible route, each node will be involved no more than two times) 
		unordered_map<StringGraphNode*, int> edge_freq{};
		for (auto edge : mst)
		{
			edge_freq[edge.sink]++;
			edge_freq[edge.source]++;
		}

		// convert into a reverse-frequency distribution (where the number of edges
		// attached to a node is called its "degree"), contains lists of nodes
		// sorted by their degree value 
		unordered_map<int, vector<StringGraphNode*>> degrees{}; 
		for (auto freq : edge_freq)
		{
			StringGraphNode* node = freq.first; 
			int node_freq = freq.second; 
			degrees[node_freq].push_back(node); 
		}

		// use a modified version of Dijkstra's algorithm that allows path-finding,
		// starting from all dead-end nodes until we find a solution 
		for (auto dead_end : degrees[1])
		{
			_reduced_graph.findFullPath(dead_end->getValue()); 
		}

		return ""; 

		/*
		// for all nodes with degree 3+, we want to find out: the paths to
		// which 2 dead ends cost the most? 

		// it's safe to assign these default values because if we have an MST
		// with at least 2 nodes, there must then be at least 2 nodes with degree 1 
		string costliest_node = degrees[1][0]->getKey(); 
		string second_costliest_node = degrees[1][1]->getKey(); 
		for (int i = edge_freq.size() - 1; i > 2; i--)
		{
			// if there exist nodes with i degrees 
			if (degrees[i].size() > 0)
			{
				// for each of these nodes, find the cost to get to all dead ends
				// (nodes with degree 1) 
				for (auto node : degrees[i])
				{
					unordered_map<string, int> paths = _reduced_graph.getShortestPaths[node->getValue()];
					for (auto dead_end : degrees[1])
					{
						if (paths[dead_end] > ) STOPPED HERE
					}
				}
			}
		}*/
	}
};

#endif //!TASKS_H