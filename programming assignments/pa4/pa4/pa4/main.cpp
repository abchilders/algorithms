/*
Assignment: PA #4
Description: This program builds a decision tree based on a supplied CSV file. 
Author: Alex Childers
HSU ID: 013145324
Completion time: 4.5 + 02:15 -
In completing this program, I received help from the following people: 
	N/A
*/

// TO DO: clarify what option 3 does
/*	-For fun, on option 3, show how many of the predictions are correct. 
		Can do this by comparing the prediction with data[outcome_column] each time before
		passing into out_stream, tallying up how many times the prediction was correct,
		and then showing [tally]/data.size(). 
*/

// createPredictions() is inefficient, due to returning a whole vector. 


#include <iostream>
#include <unordered_map>
#include <cmath>
#include <string>
#include <fstream>
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

// option 1: using CSV file and outcome variable provided by user, 
// build a decision tree 
void buildTreeFromFile(DecisionTree& tree)
{
	vector<vector<string>> data{};
	vector<string> header{};
	int outcome_col = -1; 
	getUserInput(data, header, outcome_col);

	// construct the tree
	tree.buildTree(data, header, outcome_col);

}

// recursively walks down a decision tree until a leaf node representing a 
// prediction has been reached. returns that decision
TreeNode* walkTree(
	unordered_map<string, int>& pred_indices,
	const vector<string>& instance_data,
	TreeNode* root)
{
	if (root == nullptr)
	{
		return root;
	}

	// is root a leaf node?
	if (root->children.empty() == true)
	{
		return root; 
	}
	else
	{
		// what's the index of the predictor variable we're considering?
		string predictor = root->value; 
		int pred_index = pred_indices[predictor]; 

		// what's the value of this predictor on the given data instance?
		string next_edge = instance_data[pred_index]; 

		// walk down tree to next node along this edge
		return walkTree(pred_indices, instance_data, root->children[next_edge]);
	}
}

// function to predict based on a decision tree and CSV
vector<string> createPredictions(
	 DecisionTree& tree, 
	 const vector<vector<string>>& data,
	 const vector<string>& headers,
	 const int out_column)
{
	// create a hashtable mapping predictor variables to their indices
	unordered_map<string, int> predictor_indices{}; 
	for (int i = 0; i < headers.size(); i++)
	{
		string predictor = headers[i]; 
		predictor_indices[predictor] = i; 
	}

	// a vector to hold our predictions for each row 
	vector<string> predictions{}; 

	// for each row of data, walk the tree to find its prediction
	for (auto row : data)
	{
		TreeNode* pred_node = walkTree(predictor_indices, row, tree.getRoot()); 
		predictions.push_back(pred_node->value); 
	}

	return predictions;
}

// function to output vector of predictions to CSV


// option 3: using CSV file and outcome variable provided by user, output the 
// results of the prediction from already-created decision tree to a 
// separate CSV file specified by user 
// NOTE!! CURRENTLY, THE OUTCOME COLUMN GIVEN BY THE USER IS MEANINGLESS-
//	ASK ADAM SPECIFICALLY WHAT THE PURPOSE OF THIS IS 
void predictOutcome(DecisionTree& tree)
{
	// check for valid decision tree
	if (tree.isEmpty() == true)
	{
		cout << "Error: decision tree has not been created yet." << endl;
		cout << "Please select option 1 first." << endl; 
		return; 
	}

	// get input information from user 
	vector<vector<string>> data{}; 
	vector<string> header{}; 
	int outcome_col = -1; 
	getUserInput(data, header, outcome_col); 

	// get output information from user
	string output_file = ""; 
	cout << "What is the name of the CSV file to which I can output the results?";
	getline(cin, output_file); 

	// get prediction results on data
	vector<string> predictions = createPredictions(tree, data, header, outcome_col);

	// write prediction results to output file
	ofstream out_stream{ output_file }; 
	if (out_stream.is_open() == true)
	{
		// output header
		bool needs_comma = false; 
		for (auto pred_var : header)
		{
			if (needs_comma == true)
			{
				out_stream << ",";
			}
			out_stream << pred_var; 
			needs_comma = true; 
		}
		out_stream << ",Prediction" << endl; 

		// output data with added prediction
		for (int i = 0; i < data.size(); i++)
		{
			// output existing data
			needs_comma = false; 
			for (auto datum : data[i])
			{
				if (needs_comma == true)
				{
					out_stream << ",";
				}
				out_stream << datum;
				needs_comma = true; 
			}

			// added column for prediction
			out_stream << "," << predictions[i] << endl; 
		}
	}
	out_stream.close(); 
	return; 
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

	bool keep_running = true; 
	DecisionTree initial_tree{};

	do
	{
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
			buildTreeFromFile(initial_tree);
		}
		else if (option == "2")
		{
			// TO DO 
		}

		// option 3: use the decision tree from option 1 to predict outcomes
		else if (option == "3")
		{
			predictOutcome(initial_tree); 
		}
		else if (option == "4")
		{
			// TO DO 
		}
		else
		{
			keep_running = false; 
		}

		/* Option 2: Write decision tree to file
		*/

		/* Option 3: Predict outcome
		*/

		/* Option 4: Read tree from file
		*/
	} while (keep_running == true);

	cout << "Closing program." << endl; 
	return 0; 
}