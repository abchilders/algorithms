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

// This file is written by Alex. 

#ifndef TASKS_H
#define TASKS_H

#include "CityGraph.h"
#include "CsvParser.h"
#include <iostream>

class Tasks
{
private:
	// contains our overall graph 
	CityGraph _city_graph{}; 

	// will contain a reduced graph based on the Tier 2 MST
	CityGraph _reduced_graph{}; 

	// contains a list of the nodes we want to deliver to- the int will
	// default to 1 to indicate that a node exists in this list (useful
	// when reducing the graph for Tier 2) 
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

		// calculate the total transit time on this MST
		int transit_time = 0; 
		for (auto edge : reduced_map)
		{
			transit_time += edge.weight; 
		}
		
		// output results
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
};

#endif //!TASKS_H