/*
Assignment: PA #4
Description: This program builds a decision tree based on a supplied CSV file. 
Author: Alex Childers
HSU ID: 013145324
Completion time: 9.25 hours + 17:00 - 
In completing this program, I received help from the following people: 
	N/A
*/

// -TO DO: 
/*	
	- How do I ignore unseen input?
		-i've made an attempt at this. we'll see if it works
	-For fun, on option 3, show how many of the predictions are correct. 
		Can do this by comparing the prediction with data[outcome_column] each time before
		passing into out_stream, tallying up how many times the prediction was correct,
		and then showing [tally]/data.size().  
*/

// createPredictions() is inefficient, due to returning a whole vector. 

// Note: increased stack size to some absurd amount.
// Note: if you want to expand how many rows we build a decision tree from,
//	go to line [FILL IN HERE WHEN DONE]. 




#include <iostream>
#include <unordered_map>
#include <cmath>
#include <ctime>
#include <string>
#include <fstream>
#include <queue>
#include <algorithm>
#include "CsvParser.h"
#include "DecisionTree.h"
#include "StringSplitter.h"

// may be unnecessary
#include "Menu.h"

using namespace std; 

// advice: bottom-up programming is useful
// start from what you know (invariants) and write smaller, good-quality
// functions based on these, then combine them
//	^ test-driven development: you write a test before you write code to test 
	// the truthfulness
//	of what you've coded
// I.E. start small and work your way up to create more reliable programs 

// returns a vector containing the requested number of rows, randomly selected from
// the given vector
template<typename T>
vector<T> randomReduction(const vector<T>& data, int rows)
{
	// seed the random number generator
	srand((unsigned)time(0)); 

	rows = min(data.size(), (unsigned)rows); 

	vector<T> reduced_vect{}; 
	unordered_map<int, int> selected_rows{};
	for (int i = 0; i < rows; i++)
	{
		bool found_row = false;
		while (found_row == false)
		{	
			// randomly select a row
			int random_row = rand() % rows;

			// if this row hasn't already been selected, add it to reduced_vect
			if (selected_rows[random_row] < 1)
			{
				selected_rows[random_row]++; 
				reduced_vect.push_back(data[random_row]);
				found_row = true; 
			}
		}	
	}
	return reduced_vect; 
}

// asks user for a CSV file and the outcome variable.
// populates our data matrix, header vector, and the index of the outcome
// variable accordingly
void getUserCsvInput(
	vector<vector<string>>& data, 
	vector<string> &header, 
	int& out_col,
	const string& file_purpose)
{
	string file_name = "";
	cout << "Enter the name of a CSV file " << file_purpose << ": ";
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
		cout << "What is the outcome variable? ";
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
	getUserCsvInput(data, header, outcome_col, "to build a tree from");

	// a solution to getting stack overflow on large data sets: if data.size() > 1000,
	// select 1000 random rows from data on which to create the decision tree 
	if (data.size() > 1000)
	{
		data = randomReduction(data, 1000);
	}

	// construct the tree
	tree.buildTree(data, header, outcome_col);

	cout << "Tree successfully built." << endl << endl; 
	return; 
}

// asks user for a text file name; returns this file name
string getUserTxtInput(const string& file_purpose)
{
	string file_name = "";
	cout << "Enter the name of a .txt file " << file_purpose << ": ";
	getline(cin, file_name);
	return file_name;
}

// function that walks tree, outputting it to .txt
void outputTree(DecisionTree& tree, string out_file)
{
	ofstream out_stream{ out_file };
	if (out_stream.is_open() == true)
	{
		// create a queue for level-order traversal 
		queue<pair<string, TreeNode*>> tree_nodes{};
		tree_nodes.push(pair<string, TreeNode*>{"NULL", tree.getRoot()});

		while (tree_nodes.empty() == false)
		{
			pair<string, TreeNode*> top = tree_nodes.front();
			tree_nodes.pop();

			// output node's edge value, name, and number of node's children
			string edge = top.first;
			string name = top.second->value;
			int num_children = top.second->children.size();

			out_stream << edge << "|" << name << "|" << num_children << endl;

			// push all of node's children onto queue
			for (auto kvp : top.second->children)
			{
				tree_nodes.push(pair<string, TreeNode*>{kvp.first, kvp.second});
			}
		}
	}
	out_stream.close();
	return;
}

// option 2: writes decision tree to text file 
void writeTreeToFile(DecisionTree& tree)
{
	string output_file = getUserTxtInput("to write the decision tree to");

	// walk the tree and output it to the given output_file
	outputTree(tree, output_file);

	cout << "Tree was written to " << output_file << endl << endl; 
	return;
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

		// walk down tree to next node along this edge IF the predictor value
		// exists in the tree
		if (root->children[next_edge] != nullptr)
		{
			return walkTree(pred_indices, instance_data, root->children[next_edge]);
		}
		else
		{
			return nullptr;
		}
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
		if (pred_node == nullptr)
		{
			// if we don't recognize a predictor variable value, don't 
			// make a prediction. push a placeholder in
			predictions.push_back(""); 
		}
		else
		{
			predictions.push_back(pred_node->value);
		}
	}

	return predictions;
}


// option 3: using CSV file and outcome variable provided by user, output the 
// results of the prediction from already-created decision tree to a 
// separate CSV file specified by user 
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
	getUserCsvInput(data, header, outcome_col, "on which to make predictions"); 

	// get output information from user
	string output_file = ""; 
	cout << "What is the name of a CSV file to which I can output the results? ";
	getline(cin, output_file); 

	// get prediction results on data
	vector<string> predictions = createPredictions(tree, data, header, outcome_col);

	// write prediction results to output file
	ofstream out_stream{ output_file }; 
	int correct_preds = 0; 
	int total_preds = 0; 
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

			// just for fun (not required by PA): record whether this prediction was
			// correct or not 
			total_preds++; 
			if (data[i][outcome_col] == predictions[i])
			{
				correct_preds++; 
			}
		}
	}
	out_stream.close();

	// calculate percentage of correct predictions
	double pred_percent = (double(total_preds) / data.size()) * 100;
	double percent_correct = (double(correct_preds) / double(total_preds)) * 100; 

	cout << "Predictions were written to " << output_file << endl;
	cout << "FUN STATS:" << endl;
	cout << "* Out of all rows in the file you gave me, I was able to make predictions for "
		<< pred_percent << "% of them." << endl;
	cout << "* Of those " << pred_percent << "%, I predicted " << percent_correct 
		<< "% correctly." << endl << endl; 
	return; 
}

// given an input_file name, this reads in the file, creates a decision
// tree from the file, and returns a pointer to the root of the tree 
TreeNode* treeFromText(string in_file)
{
	TreeNode* root{};

	// open input stream
	ifstream in_stream{ in_file }; 
	if (in_stream.is_open() == true)
	{
		// read in initial line of data
		string next_line = "";
		getline(in_stream, next_line);
		vector<string> node_data = StringSplitter::split(next_line, "|");

		// initialize tree, assuming first edge is "NULL"
		string name = node_data[1];
		int num_children = stoi(node_data[2]);
		root = new TreeNode;
		root->value = name;

		// start a queue for level-order tree creation
		queue<pair<TreeNode*, int>> tree_nodes{}; 
		tree_nodes.push(pair<TreeNode*, int>(root, num_children)); 
		while (tree_nodes.empty() == false)
		{
			pair<TreeNode*, int> current = tree_nodes.front(); 
			TreeNode* curr_node = current.first;
			int num_children = current.second; 
			tree_nodes.pop(); 

			// create children of current node, pushing them onto queue for processing
			// if they have children
			for (int i = 0; i < num_children; i++)
			{
				// read in next line 
				string next_line = ""; 
				getline(in_stream, next_line); 
				vector<string> node_data = StringSplitter::split(next_line, "|");

				string edge = node_data[0];
				string name = node_data[1];
				int next_num_children = stoi(node_data[2]); 

				// create a node for this child
				TreeNode* child = new TreeNode;
				child->value = name; 

				// connect child to parent
				curr_node->children[edge] = child; 

				// if the child node has any children, push it onto the queue 
				if (next_num_children > 0)
				{
					tree_nodes.push(pair<TreeNode*, int>(child, next_num_children)); 
				}
			}
		}
		
	}
	in_stream.close(); 
	return root; 
}

// option 4: build an in-memory decision tree representation of a 
// user-specified file 
DecisionTree readTreeFromFile()
{
	// get user input 
	string file_name = getUserTxtInput("to build a tree from"); 

	// construct tree from input file 
	DecisionTree tree{ treeFromText(file_name) };
	cout << "Tree was constructed from " << file_name << endl << endl; 
	return tree; 
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
	DecisionTree program_tree{};

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
			buildTreeFromFile(program_tree);
		}

		// option 2: write decision tree to file 
		else if (option == "2")
		{
			writeTreeToFile(program_tree); 
		}

		// option 3: use the decision tree from option 1 to predict outcomes
		else if (option == "3")
		{
			predictOutcome(program_tree); 
		}

		// option 4: read tree from file 
		else if (option == "4")
		{
			program_tree = readTreeFromFile(); 
		}
		else
		{
			keep_running = false; 
		}
	} while (keep_running == true);

 	cout << "Closing program." << endl; 
	return 0; 
}