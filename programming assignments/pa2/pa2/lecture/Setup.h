#pragma once
#ifndef SETUP_H
#define SETUP_H

#include "CampusGraph.h"

class Setup
{
public:
	// Takes a vector of CSV data representing time between campus buildings.
	// Creates a campus graph with the given data. 
	static void createGraph(const vector<vector<string>>& campus_data, 
							CampusGraph& graph)
	{
		// for each row in the given data
		for (auto edge_info : campus_data)
		{
			string start = edge_info[0]; 
			string end = edge_info[1];
			int weight = stoi(edge_info[2]); 
		}
	}
};

#endif // !SETUP_H