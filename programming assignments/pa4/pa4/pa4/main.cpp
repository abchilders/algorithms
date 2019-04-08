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

// asks user for a CSV file and the outcome variable.
// populates our data matrix, header vector, and the index of the outcome
// variable accordingly
void getUserInput(
	vector<vector<string>>& data, 
	vector<string> &header, 
	int& out_col)
{
	string file_name = "";
	cout << "Enter the name of a CSV file: ";
	getline(cin, file_name);

	// parse file input
	CsvStateMachine parser{ file_name };
	data = parser.processFile();

	// move the header vector to its own vector, leaving data as a vector 
	// of vectors with pure data
	header = move(data[0]);
	data.erase(data.begin());

	// assuming the user's outcome variable isn't an empty string,
	// validate it
	string outcome_var = "";
	do
	{
		cout << "What is the outcome variable?";
		getline(cin, outcome_var);

		// validate that outcome_var exists
		auto valid_outcome = find(header.begin(), header.end(), outcome_var);
		if (valid_outcome != header.end())
		{
			out_col = distance(header.begin(), valid_outcome);
		}
		else
		{
			cout << "Invalid predictor variable. Try again." << endl;
			outcome_var = "";
		}
	} while (outcome_var == "");
}

void buildTreeFromFile(DecisionTree& tree)
{
	vector<vector<string>> data{};
	vector<string> header{};
	int outcome_col = -1; 
	getUserInput(data, header, outcome_col);

	// construct the tree
	tree.buildTree(data, header, outcome_col);

}

int main(void)
{
	/*
	CsvStateMachine parser{ "easy data set.csv" };
	vector<vector<string>> data{}; 
	data = parser.processFile(); 
	vector<string> header = data[0]; 

		/*
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

	DecisionTree tree{};

	// present menu options for the user
	cout << "Enter the number of the task you'd like to perform: " << endl;
	cout << "1: Build decision tree from file " << endl;
	cout << "2: Write decision tree to file " << endl;
	cout << "3: Predict outcome " << endl; 
	cout << "4: Read tree from file " << endl; 
	cout << "(or enter anything else to exit)" << endl; 
	string option = ""; 
	cin >> option; 
	cin.ignore(); 

	// option 1: build a decision tree from user-supplied file
	if (option == "1")
	{
		buildTreeFromFile(tree); 
	}
	else if (option == "2")
	{
		// TO DO 
	}
	else if (option == "3")
	{
		// TO DO 
	}
	else if (option == "4")
	{
		// TO DO 
	}
	else
	{
		cout << "Closing program. " << endl; 
		return 0; 
	}

	/* Option 2: Write decision tree to file
	*/

	/* Option 3: Predict outcome
	*/

	/* Option 4: Read tree from file 
	*/

	return 0; 
}