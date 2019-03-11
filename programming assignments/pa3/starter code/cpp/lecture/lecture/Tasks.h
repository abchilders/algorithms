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
	vector<string> _deliveries{}; 

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

		// record deliveries
		parseDeliveries(dest_file); 

		// Task 2: reduce the graph using Dijkstra's algorithm, then create an MST 
		// on that reduced graph 
		
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
			if (_city_graph.nodeExists(first_house) == false)
			{
				_city_graph.addVertex(first_house); 
			}
			if (_city_graph.nodeExists(second_house) == false)
			{
				_city_graph.addVertex(second_house); 
			}

			// connect bidirectionally 
			_city_graph.connectVertex(first_house, second_house, weight, true); 
		}
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
				_deliveries.push_back(next_house); 
			}
		}
		dest_stream.close(); 
		
	}
};

#endif //!TASKS_H