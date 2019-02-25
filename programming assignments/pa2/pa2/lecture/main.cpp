/*
Assignment: PA #2 - Dijkstra's algorithm
Description: This program will use CSV information on campus walking times
	to prompt the user for two campus location codes and calculate the 
	estimated shortest travel time using Dijkstra's algorithm. 
Author: Alex Childers
HSU ID: 013145324
Completion time: 7.75
In completing this program, I received help from the following people:
	N/A
*/

// For Python: open in Visual Studio Code.

// NOTE: changed Project Options -> C/C++ -> General -> Debug Information format -> C7, not program database for edit and continue

/* TO DO:
	- Make it so that we don't have to recalculate shortest paths every time we 
		want to look up a new node. 
*/

#include "CsvParser.h"
#include "Tasks.h"

// holds data on how long it takes to get from node to node
const string DISTANCE_FILE = "distances.csv"; 

// the names of all buildings on campus, their abbreviations, and their node locations
const string CODES_FILE = "facility_codes.csv"; 

// the names assigned to each node from distances.csv
const string NODE_NAMES_FILE = "node_names.csv"; 

// NOTE: in data.csv, format is [start], [end], [weight]
// To create bidirectional edges, must connect as well with [end], [start], [weights]
	// Weights may differ depending on whether you go up or down stairs, etc. 

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

	// building graph
   CampusGraph graph{};
   graph.addVertex("a"); 
   graph.addVertex("b");
   graph.addVertex("c");

   // A is connected to B bidirectionally 
   graph.connectVertex("a", "b", 3, true); 

   // A is connected to C unidirectionaly
   graph.connectVertex("a", "c", 15);

   // B and C are connected bidirectionally
   graph.connectVertex("b", "c", 7, true);

   // should return b, 3; c, 10
   auto distances = graph.computeShortestPath("a"); 

   // note that auto is a compiler-time thing where the computer substitutes return type
   // in for auto, automatically.
   // dynamic casting is a runtime thing. 

   return 0;
}