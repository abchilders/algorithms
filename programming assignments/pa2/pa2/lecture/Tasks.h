#pragma once
#ifndef TASKS_H
#define TASKS_H

#include "CampusGraph.h"
#include <iostream>

class Tasks
{
private:
	// converts a number of seconds to a string representing minutes and seconds
	static string convertToMinutes(int seconds)
	{
		ostringstream result{};
		result << (seconds / 60) << " minutes and " << (seconds % 60) << " seconds";
		return result.str();
	}

public:

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

		// using node_mapping, start building the graph
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
			
			// connect unidirectionally, because I figure that weights may be affected
			// by whether one is going up or down a hill, for instance - may cause 
			// inaccuracies, depending on gaps in knowledge from our graph as collected 
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
	// it takes to travel between those locations to the screen & the path to and from.
	static void userInterface(CampusGraph* the_graph)
	{
		// get user input
		string start_loc = "";
		string end_loc = "";

		cout << "**HSU Transit Time Calculator**" << endl;
		cout << "Enter starting location: ";
		cin >> start_loc; 
		transform(start_loc.begin(), start_loc.end(), start_loc.begin(), ::toupper); 
		cout << "Enter destination: ";
		cin >> end_loc;
		transform(end_loc.begin(), end_loc.end(), end_loc.begin(), ::toupper); 

		// validate user input
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
			// if user input is valid, calculate time to get to all other nodes from start_loc
			unordered_map<string, Path<string>> shortest_paths{};
			shortest_paths = the_graph->computeShortestPath(start_name);

			// output estimated travel time
			cout << "Estimated travel time: " << convertToMinutes(shortest_paths[end_name].getWeight()) << endl; 
			cout << "On your way from " << the_graph->getBuildingName(start_loc)
				<< " to " << the_graph->getBuildingName(end_loc)
				<< ", you will pass by: "; 

			// output path from start to end
			vector<string> path = shortest_paths[end_name].getPath(); 

			if (path.size() < 3)
			{
				// the path only consists of the start point and the end point at most
				cout << "no other buildings" << endl; 
			}
			else
			{
				bool needs_comma = false;

				// the path has at least one intermediate building between start and end
				for (int i = 1; i < path.size() - 1; i++)
				{
					if (needs_comma == true)
					{
						cout << ", ";
					}
					cout << path[i];
					needs_comma = true;
				}
				cout << endl;
			}
		}
		return; 
	}
};

#endif // !TASKS_H