#include "Tasks.h"
#include <iostream>

void Tasks::userInterface()
{
	// get user input
	string city_map_file = "";
	string dest_file = ""; 
	cout << "***Route Planner***" << endl; 
	cout << "Enter maps file: "; 
	getline(cin, city_map_file); 
	cout << "Enter destinations file: "; 
	getline(cin, dest_file); 

	// DO ALL THE BEHIND THE SCENES WORK HERE
	createGraph(city_map_file); 

	// output results
	string transit_time = ""; 
	cout << "Total transit time: " << transit_time << endl; 
}

void Tasks::createGraph(string map_file)
{
	// parse CSV map_file
	CsvStateMachine map_csm{ map_file }; 
	vector<vector<string>> map_data = map_csm.processFile(); 

	// create bidirectional graph 
	for (auto edge_info : map_data)
	{
		string first_house = edge_info[0]; 
		string second_house = edge_info[1]; 
		int weight = stoi(edge_info[2]); 
	}
}