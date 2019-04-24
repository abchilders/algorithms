#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "WordStorage.h"

int WordStorage::calculateEditDistance(const string& first, const string& second)
{
	vector<vector<int>> matrix{};

	//build result matrix
	matrix.resize(first.length() + 1);
	for (auto& row : matrix)
	{
		row.resize(second.length() + 1);
	}

	//fill in first row  
	for (int i = 0; i < matrix[0].size(); i++)
	{
		matrix[0][i] = i;
	}

	//fill in first column
	for (int i = 0; i < matrix.size(); i++)
	{
		matrix[i][0] = i;
	}

	//compute rest of matrix
	for (int i = 1; i < matrix.size(); i++)
	{
		for (int j = 1; j < matrix[i].size(); j++)
		{
			//find least cost of our 3 choices
			int top_cost = matrix[i - 1][j] + 1;
			int left_cost = matrix[i][j - 1] + 1;
			int diagonal_cost = matrix[i - 1][j - 1];

			//add 1 if characters are not the same
			if (first[i - 1] != second[j - 1])
			{
				diagonal_cost++;
			}

			int best_choice = min(top_cost, min(left_cost, diagonal_cost));



			//store result in current cell
			matrix[i][j] = best_choice;
		}
	}

	return matrix[matrix.size() - 1][matrix[0].size() - 1];
}

void WordStorage::getUserStorage()
{
	//contains our dictionary 
	vector<string> dictionary{};

	// read the words.txt dictionary into the program 
	ifstream words{ _DICTIONARY };
	if (words.is_open() == true)
	{
		while (words.good() == true)
		{
			// read dictionary words into vector
			string next_word = "";
			words >> next_word;
			dictionary.push_back(next_word);
		}
	}
	words.close();

	// prompt the user for basic file store or load/store in database
	string storage_method = "";
	cout << "Would you like to store autocorrect results to a file or to "
		<< "a database? " << endl;
	cout << "Enter 1 for file, or 2 for database" << endl;
	getline(cin, storage_method);

	// for now, we'll just default to file storage until DB functionality
	// is implemented
	if (storage_method == "2")
	{
		cout << "Database functionality not available yet. Will store "
			<< "autocorrect results to a file." << endl;
	}
}