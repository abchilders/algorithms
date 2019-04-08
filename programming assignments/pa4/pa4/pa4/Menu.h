#pragma once
#ifndef MENU_H
#define MENU_H

#include <iostream>
#include "DecisionTree.h""

using namespace std; 

class Menu
{
private:
	DecisionTree* _tree{}; 
	vector<vector<string>>* _data{}; 
	vector<string>* _header{}; 
	int _outcome_col = -1; 

public:
	/*
	// asks user for a CSV file and the outcome variable.
	// populates the information we need to build a decision tree accordingly
	void getUserCsvInput()
	{
		string file_name = ""; 
		cout << "Enter the name of a CSV file: "; 
		getline(cin, file_name); 

		// parse file input
		CsvStateMachine parser{ file_name };
		_data = &parser.processFile();

		// move the header vector to its own vector, leaving data as a vector 
		// of vectors with pure data
		_header = move(_data[0]);
		_data.erase(_data.begin());

		// assuming the user's outcome variable isn't an empty string,
		// validate it
		string outcome_var = ""; 
		do
		{
			cout << "What is the outcome variable?";
			getline(cin, outcome_var);

			// validate that outcome_var exists
			auto valid_outcome = find(_header.begin(), _header.end(), outcome_var);
			if (valid_outcome != _header.end())
			{
				_outcome_col = distance(_header.begin(), valid_outcome); 
			}
			else
			{
				cout << "Invalid predictor variable. Try again." << endl; 
				outcome_var = ""; 
			}
		} while (outcome_var == "");
	}

	void buildTreeFromFile()
	{
		getUserCsvInput(); 

		// construct the tree
		_tree->buildTree(_data, _header, _outcome_col); 

	} */
};

#endif //! MENU_H