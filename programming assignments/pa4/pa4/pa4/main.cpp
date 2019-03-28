/*
Assignment: PA #4
Description: This program builds a decision tree based on a supplied CSV file. 
Author: Alex Childers
HSU ID: 013145324
Completion time: 0.5 hours
In completing this program, I received help from the following people: 
	N/A
*/



#include <iostream>
#include <unordered_map>
#include <cmath>
#include <string>
#include "CsvParser.h"
#include "TreeNode.h"

using namespace std; 

// advice: bottom-up programming is useful
// start from what you know (invariants) and write smaller, good-quality
// functions based on these, then combine them
//	^ test-driven development: you write a test before you write code to test 
	// the truthfulness
//	of what you've coded
// I.E. start small and work your way up to create more reliable programs 

// calculate entropy based on a frequency distribution of all outcome
// levels 
						// freq. dist. 
						// each item represents 
double calculateEntropy(const unordered_map<string, int>& outcome_level)
{
	// determine denominator
	int denominator = 0;
	for (auto i : outcome_level)
	{
		denominator += i.second;
	}

	// calculate entropy
	double entropy = 0.0;
	for (auto item : outcome_level)
	{
		double ratio = (double)item.second / denominator;
		double logged = log2(ratio);
		entropy += -ratio * logged;
	}
	return entropy;
}

// builds frequency distribution based on supplied vector of data
unordered_map<string, int> buildFrequencyDistribution(const vector<string>& data)
{
	unordered_map<string, int> distribution{};
	for (auto item : data)
	{
		distribution[item]++;
	}
	return distribution;
}

// tallies up observations from one column of the data matrix
// allows us to grab a specific column of data from our 2d matrix 
vector<string> getObservations(const vector<vector<string>>& matrix, int column)
{
	vector<string> result{};
	for (int i = 0; i < matrix.size(); i++)
	{
		result.push_back(matrix[i][column]);
	}
	return result;
}

// for the given matrix, reduce matrix such that all results
// match the supplied predictor variable on the supplied column
vector<vector<string>> reduceMatrix(
	const vector<vector<string>>& matrix, // what we're reducing. our matrix is row-major,
		// which means that each ROW represents one unique item, and each column represents
		// an attribute of the items 
	int column, // the index of the column we're reducing on 
	string predictor // the value of the predictor variable we reduce on
)
{
	vector<vector<string>> result{};
	int row_counter = 0;
	for (int i = 0; i < matrix.size(); i++)
	{
		if (matrix[i][column] == predictor)
		{
			result.push_back(matrix[i]);
		}
	}
	return result;
}

// HEY!!! THIS IS WRONG. DIFF IT ON ADAM'S BEFORE PROCEEDING 
// returns which column gave us the most gain 
int findMaxGain(
	const vector<vector<string>>& matrix, 
	int outcome_column, 
	double entropy)
{
	// if the matrix has no data, exit
	if (matrix.size() == 0)
	{
		return -1;
	}

	vector<double> information_gain{};

	// calculate information gain for each predictor variable 
	for (int column = 0; column < matrix[0].size(); column++)
	{
		// skip outcome column 
		if (column == outcome_column)
		{
			information_gain.push_back(-1); 
			continue; 
		}

		vector<string> observations = getObservations(matrix, column);
		unordered_map<string, int> observation_levels = buildFrequencyDistribution(observations); 
		double local_entropy = 0.0; 

		// reduce table to observations that match a given value (e.g. overcast) 
		for (auto level : observation_levels)
		{
			auto reduced_matrix = reduceMatrix(matrix, column, level.first); 

			// reduce observations on a certain outcome 
			auto reduced_observations = getObservations(reduced_matrix, outcome_column); 
			local_entropy += ((double)level.second / observations.size()) *
				calculateEntropy(buildFrequencyDistribution(reduced_observations));
		}
		information_gain.push_back(entropy - local_entropy); 
	}

	// return column with most gain
	// to start, assume that the first column has the most gain
	int most_gain = 0;
	for (int i = 1; i < information_gain.size(); i++)
	{
		if (information_gain[i] > information_gain[most_gain])
		{
			most_gain = i;
		}
	}
	return most_gain; 
}

TreeNode* buildTree(
	const vector<vector<string>>& matrix,
	const vector<string>& predictors,
	const int outcome_column
)
{
	// calculate entropy of entire observation set on outcome variable 
	vector<string> observations = getObservations(matrix, outcome_column);
	double entropy = calculateEntropy(buildFrequencyDistribution(observations));

	// base case: 0 entropy
	if (entropy < 0.01)
	{
		TreeNode* node = new TreeNode{}; 
		node->value = matrix[0][outcome_column];
		return node; 
	}

	int col = findMaxGain(matrix, outcome_column, entropy);

	// create new node
	TreeNode* node = new TreeNode{}; 
	node->value = predictors[col]; 

	// attach as children all levels of outcome
	vector<string> selected_observations = getObservations(matrix, col); 
	auto selected_levels = buildFrequencyDistribution(selected_observations); 

	// for each predictor 
	for (auto level : selected_levels)
	{
		auto reduced_matrix = reduceMatrix(matrix, col, level.first);
		node->children[level.first] = buildTree(reduced_matrix, predictors, outcome_column); 
	}
	return node; 
}

int main(void)
{
	CsvStateMachine parser{ "easy data set.csv" };
	vector<vector<string>> data = parser.processFile(); 
	vector<string> header = data[0]; 

	// NOTE: very slow! 
	// to make this more algorithmically efficient, PLEASE use STL move  
	// to move array elements 1...size() to another structure --> near-instant, says Adam 
	data.erase(data.begin()); 

	TreeNode* root = buildTree(data, header, 4); 

	// My job: data management 
	/* Prompt user for predictor column
	Run prediction algorithm on 20 % CSV
	Re-run on 80% and see if it matches, and if so, how often was it correct? 
	*/

	return 0; 
}