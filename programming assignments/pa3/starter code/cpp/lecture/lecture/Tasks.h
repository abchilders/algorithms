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
	CityGraph the_graph{}; 

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
		createGraph(city_map_file);
		

		// output results
		string transit_time = "";
		cout << "Total transit time: " << transit_time << endl;
	}

	// uses a CSV map file to create a CityGraph
	void createGraph(string map_file)
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

			// add the houses to the graph if they don't already exist
			if (the_graph.nodeExists(first_house) == false)
			{
				the_graph.addVertex(first_house); 
			}
			if (the_graph.nodeExists(second_house) == false)
			{
				the_graph.addVertex(second_house); 
			}

			// connect bidirectionally 
			the_graph.connectVertex(first_house, second_house, weight, true); 
		}
	}
};

#endif //!TASKS_H