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
	static CampusGraph* createGraph(
		const vector<vector<string>>& campus_data, 
		const vector<vector<string>>& buildings,
		const vector<vector<string>>& nodes
	)
	{
		// to start, let's make a HT connecting node names from campus_data 
		// (letters of the alphabet) to what humans usually call them
		// (e.g. A --> Canyon Complex)
		unordered_map<string, string> node_mapping{}; 
		for (auto node_aliases : nodes)
		{
				// alphabetical letter    full name
			node_mapping[node_aliases[0]] = node_aliases[1];
		}

		// now, using this HT, start building the graph
		CampusGraph* the_graph = new CampusGraph{};

		// for each row in the given distance data
		for (auto edge_info : campus_data)
		{
			// remember: full node names can be looked up by their alphabetical
			// alias
			string start = node_mapping[edge_info[0]]; 
			string end = node_mapping[edge_info[1]];
			int weight = stoi(edge_info[2]); 

			// add the vertices to the graph if they don't already exist 
			if (the_graph->nodeExists(start) == false)
			{
				the_graph->addVertex(start);
			}
			if (the_graph->nodeExists(end) == false)
			{
				the_graph->addVertex(end);
			}
			
			// connect unidirectionally
			the_graph->connectVertex(start, end, weight); 
		}

		// then add information on each building abbreviation, for use in the
		// user interface
		for (auto building : buildings)
		{
			the_graph->addBuilding(building[0], building[1], node_mapping[building[2]]); 
		}

		return the_graph; 
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
		string start_name = the_graph->getBuildingNodeName(start_loc); 
		string end_name = the_graph->getBuildingNodeName(end_loc); 
		if (the_graph->nodeExists(start_name) == false)
		{
			cout << start_loc << " is an invalid starting location. " << endl; 
		}
		else if (the_graph->nodeExists(end_name) == false)
		{
			cout << end_loc << " is an invalid destination." << endl;
		}
		else
		{
			unordered_map<string, int> shortest_paths{};
			shortest_paths = the_graph->computeShortestPath(start_name);
			cout << "Estimated travel time: " << convertToMinutes(shortest_paths[end_name]);
			cout << "On your way from " << the_graph->getBuildingName(start_loc)
				<< " to " << the_graph->getBuildingName(end_loc)
				<< ", you will pass by: "; 

			// get the path from the end node
			// FINISH NEXT TIME
		}
		return; 
	}

	// converts a number of seconds to a string representing minutes and seconds
	static string convertToMinutes(int seconds)
	{
		ostringstream result{};
		result << (seconds / 60) << " minutes and " << (seconds % 60) << " seconds";
		return result.str(); 
	}

	/* MIGHT BE USEFUL FOR WRITING TO/FROM SHORTEST PATH FILE, BUT USELESS FOR NOW
	// Takes a CampusGraph that was previously created. Calculates how long it 
	// takes to go from start to all other nodes.
	static void calculatePath(CampusGraph* the_graph, string start, string end)
	{
		// compute the shortest path using Dijkstra's algorithm
		unordered_map<string, int> shortest_paths{};
		shortest_paths = the_graph->computeShortestPath(start);
	}
	*/
};

#endif // !TASKS_H