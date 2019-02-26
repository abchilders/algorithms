/*
Assignment: PA #2 - Dijkstra's algorithm
Description: This program will use CSV information on campus walking times
	to prompt the user for two campus location codes and calculate the 
	estimated shortest travel time using Dijkstra's algorithm, as well as
	outputting what buildings one will pass on the way. 
Author: Alex Childers
HSU ID: 013145324
Completion time: 8.75 + 19:00 - 
In completing this program, I received help from the following people:
	N/A
*/

#include "CsvParser.h"
#include "Tasks.h"

// holds data on how long it takes to get from node to node
const string DISTANCE_FILE = "distances.csv"; 

// the names of all buildings on campus, their abbreviations, and their node locations
const string CODES_FILE = "facility_codes.csv"; 

// the names assigned to each node from distances.csv
const string NODE_NAMES_FILE = "node_names.csv"; 

int main(void)
{
	// Parsing CSV files for graph building
	CsvStateMachine distances_csm{ DISTANCE_FILE }; 
	CsvStateMachine codes_csm{ CODES_FILE }; 
	CsvStateMachine nodes_csm{ NODE_NAMES_FILE }; 

	// vector of CSV rows
	vector<vector<string>> data = distances_csm.processFile(); 
	vector<vector<string>> facility_codes = codes_csm.processFile(); 
	vector<vector<string>> node_names = nodes_csm.processFile();

	// create unidirectional graph and add building abbr. information
	CampusGraph* hsu_graph = Tasks::createGraph(data, facility_codes, node_names); 

	// calculate travel time
	Tasks::userInterface(hsu_graph); 

   return 0;
}