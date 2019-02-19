/*
Assignment: PA #2 - Dijkstra's algorithm
Description: This program will use CSV information on campus walking times
	to prompt the user for two campus location codes and calculate the 
	estimated shortest travel time using Dijkstra's algorithm. 
Author: Alex Childers
HSU ID: 013145324
Completion time: 1.5 + 
In completing this program, I received help from the following people:
	N/A
*/

// For Python: open in Visual Studio Code.

/* TO DO:
	- Make it so that we don't have to recalculate shortest paths every time we 
		want to look up a new node. 
	- The graph isn't aware of its edges when we create it. Fix this. 
*/

#include "CsvParser.h"
#include "Tasks.h"

// NOTE: in data.csv, format is [start], [end], [weight]
// To create bidirectional edges, must connect as well with [end], [start], [weights]
	// Weights may differ depending on whether you go up or down stairs, etc. 

int main(void)
{
	// Example of how to parse a CSV file for graph building
	CsvStateMachine csm{ "distances.csv" }; 

	// vector of CSV rows
	vector<vector<string>> data = csm.processFile(); 

	// create graph -- unidirectionally connected
	CampusGraph* hsu_graph = new CampusGraph{};
	Tasks::createGraph(data, hsu_graph); 

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