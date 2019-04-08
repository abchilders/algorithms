#pragma once
#ifndef MENU_H
#define MENU_H

#include <iostream>

using namespace std; 

class Menu
{
	static void getUserInput()
	{
		string file_name = ""; 
		cout << "Enter the name of a CSV file: "; 
		cin >> file_name; 

		// parse file input
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
	}
};

#endif //! MENU_H