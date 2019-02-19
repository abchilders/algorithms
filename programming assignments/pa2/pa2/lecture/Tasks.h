#pragma once
#ifndef TASKS_H
#define TASKS_H

#include "CampusGraph.h"
#include <iostream>

class Tasks
{
public:
	// Takes a vector of CSV data representing time between campus buildings.
	// Creates a campus graph with the given data. 
	static void createGraph(const vector<vector<string>>& campus_data,
									CampusGraph* the_graph)
	{
		// for each row in the given data
		for (auto edge_info : campus_data)
		{
			string start = edge_info[0]; 
			string end = edge_info[1];
			int weight = stoi(edge_info[2]); 

			// add the vertices to the graph and connect unidirectionally
			the_graph->addVertex(start); 
			the_graph->addVertex(end); 
			the_graph->connectVertex(start, end, weight); 
		}
	}

	// Interfaces with user. Gets start and end locations; prints the amount of time
	// it takes to travel between those locations to the screen. 
	static void userInterface(CampusGraph* the_graph)
	{
		// get user input
		string start_loc = "";
		string end_loc = "";

		cout << "**HSU Transit Time Calculator**" << endl;
		cout << "Enter starting location: ";
		cin >> start_loc;
		cout << "Enter destination: ";
		cin >> end_loc;

		// calculate time to get to all other nodes from start_loc
		// HEY!!! MAKE THIS MORE EFFICIENT LATER 
		unordered_map<string, int> shortest_paths{}; 
		shortest_paths = the_graph->computeShortestPath(start_loc); 
		cout << "Estimated travel time: " << convertToMinutes(shortest_paths[end_loc]);
		return; 
	}

	// converts a number of seconds to a string representing minutes and seconds
	static string convertToMinutes(int seconds)
	{
		ostringstream result{};
		result << (seconds / 60) << " minutes and " << (seconds % 60) << " seconds";
		return result.str(); 
	}

	// Takes a CampusGraph that was previously created. Calculates how long it 
	// takes to go from start to all other nodes.
	static void calculatePath(CampusGraph* the_graph, string start, string end)
	{
		// compute the shortest path using Dijkstra's algorithm
		unordered_map<string, int> shortest_paths{};
		shortest_paths = the_graph->computeShortestPath(start);
	}
};

#endif // !TASKS_H