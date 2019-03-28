// All member functions are (CURRENTLY) written by Adam Carter. 

#pragma once
#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include "TreeNode.h"

class DecisionTree
{
private:
	TreeNode* _tree{}; 

public:
	// calculate entropy based on a frequency distribution of all outcome
	// levels 
	double calculateEntropy(const unordered_map<string, int>& outcome_level);

	// builds frequency distribution based on supplied vector of data
	unordered_map<string, int> buildFrequencyDistribution(
		const vector<string>& data);

	// tallies up observations from one column of the data matrix
	// allows us to grab a specific column of data from our 2d matrix 
	vector<string> getObservations(
		const vector<vector<string>>& matrix, int column);

	// for the given matrix, reduce matrix such that all results
	// match the supplied predictor variable on the supplied column
	vector<vector<string>> reduceMatrix(
		const vector<vector<string>>& matrix,
		int column,
		string predictor
	);

	// returns which column gave us the most gain 
	int findMaxGain(
		const vector<vector<string>>& matrix,
		int outcome_column,
		double entropy);

	// uses the given information to build a decision tree with root _tree
	void buildTree(
		const vector<vector<string>>& matrix,
		const vector<string>& predictors,
		const int outcome_column
	);

	// starts building a decision tree from the root
	// builds a decision tree
	TreeNode* buildTreeHelper(
		const vector<vector<string>>& matrix,
		const vector<string>& predictors,
		const int outcome_column
	);
};

#endif //!DECISION_TREE_H