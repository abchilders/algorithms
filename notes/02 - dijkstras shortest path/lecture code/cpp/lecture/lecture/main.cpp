// For Python: open in Visual Studio Code.

#include "CampusGraph.h"
#include "CsvParser.h"

// NOTE: in data.csv, format is [start], [end], [weight]
// To create bidirectional edges, must connect as well with [end], [start], [weights]
	// Weights may differ depending on whether you go up or down stairs, etc. 

int main(void)
{
	// Example of how to parse a CSV file for graph building
	CsvStateMachine csm{ "data.csv" }; 
	vector<vector<string>> data = csm.processFile(); 

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