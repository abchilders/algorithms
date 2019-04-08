/*
Assignment: PA #4
Description: This program builds a decision tree based on a supplied CSV file. 
Author: Alex Childers
HSU ID: 013145324
Completion time: 1.5 hours + 23:00 - 
In completing this program, I received help from the following people: 
	N/A
*/



#include <iostream>
#include <unordered_map>
#include <cmath>
#include <string>
#include "CsvParser.h"
#include "DecisionTree.h"
#include "Menu.h"

using namespace std; 

// advice: bottom-up programming is useful
// start from what you know (invariants) and write smaller, good-quality
// functions based on these, then combine them
//	^ test-driven development: you write a test before you write code to test 
	// the truthfulness
//	of what you've coded
// I.E. start small and work your way up to create more reliable programs 

int main(void)
{
	CsvStateMachine parser{ "easy data set.csv" };
	vector<vector<string>> data = parser.processFile(); 
	//vector<string> header = data[0]; 

	// NOTE: very slow! 
	// to make this more algorithmically efficient, PLEASE use STL move  
	// to move array elements 1...size() to another structure --> near-instant, says Adam 
	//data.erase(data.begin()); 

	// move the header vector to its own vector, leaving data as a vector 
	// of vectors with pure data
	vector<string> header{};
	header = move(data[0]);
	data.erase(data.begin()); 

	// build a decision tree from the given data & header, where 
	// header[4] is the outcome variable
	DecisionTree tree{}; 
	tree.buildTree(data, header, 4); 

	// My job: data management 
	/* Prompt user for predictor column
	Run prediction algorithm on 20 % CSV
	Re-run on 80% and see if it matches, and if so, how often was it correct? 
	*/

	// present menu options for the user
	cout << "Enter the number of the task you'd like to perform: " << endl;
	cout << "1. Build decision tree from file " << endl;
	cout << "2. Write decision tree to file " << endl;
	cout << "3. Predict outcome " << endl; 
	cout << "4. Read tree from file " << endl; 

	/* Tier 1: Build decision tree from file 
	*/
	// prompt the user for a CSV file 
	string file_name = "";
	string outcome_var = ""; 
	cout << "Enter the name of a CSV file: ";
	cin >> file_name; 
	cout << "What is the outcome variable? "; 
	cin >> outcome_var; 

	/* Tier 2: Write decision tree to file
	*/

	/* Tier 3: Predict outcome
	*/

	/* Tier 4: Read tree from file 
	*/

	return 0; 
}