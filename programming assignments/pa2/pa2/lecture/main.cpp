/*
Assignment: PA #2 - Dijkstra's algorithm
Description: This program will use CSV information on campus walking times
	to prompt the user for two campus location codes and calculate the 
	estimated shortest travel time using Dijkstra's algorithm. 
Author: Alex Childers
HSU ID: 013145324
Completion time: 0.5 + 
In completing this program, I received help from the following people:
	N/A
*/

// For Python: open in Visual Studio Code.

/* TO DO:
-change input file from data.csv to whatever the real results CSV is
*/

#include "CampusGraph.h"
#include "CsvParser.h"
#include <iostream>

// NOTE: in data.csv, format is [start], [end], [weight]
// To create bidirectional edges, must connect as well with [end], [start], [weights]
	// Weights may differ depending on whether you go up or down stairs, etc. 

int main(void)
{
	// Example of how to parse a CSV file for graph building
	CsvStateMachine csm{ "data.csv" }; 

	// vector of CSV rows
	vector<vector<string>> data = csm.processFile(); 

	// create graph -- for our purposes, we'll assume places on the graph
	// are bidirectionally connected
	// DESIGN DECISION: i think pass by reference will minimize complexity?

	// get user input
	string start_loc = ""; 
	string end_loc = ""; 

	cout << "**HSU Transit Time Calculator**" << endl; 
	cout << "Enter starting location: ";
	cin >> start_loc; 
	cout << "Enter destination: ";
	cin >> end_loc; 

	// calculate travel time

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