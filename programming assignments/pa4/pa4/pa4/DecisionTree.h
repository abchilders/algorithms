// All member functions are written by Adam Carter. 
// I rewrote buildTree() into buildTree() and buildTreeHelper().
// I also wrote getRoot() and isEmpty(), and constructors. 

#pragma once
#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include "TreeNode.h"


class DecisionTree
{
private:
	TreeNode* _root{};

public:
	// default constructor
	DecisionTree()
	{
		_root = nullptr; 
	}

	// constructor
	DecisionTree(TreeNode* new_root)
	{
		_root = new_root; 
	}

	// returns the root of the decision tree
	TreeNode* getRoot()
	{
		return _root;
	}
	
	// returns true if the decision tree contains nothing 
	bool isEmpty()
	{
		if (_root == nullptr)
		{
			return true; 
		}
		else
		{
			return false; 
		}
	}

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

	TreeNode* buildTreeHelper(
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
			node->children[level.first] = buildTreeHelper(reduced_matrix, predictors, outcome_column);
		}
		return node;
	}

	void buildTree(
		const vector<vector<string>>& matrix,
		const vector<string>& predictors,
		const int outcome_column
	)
	{
		_root = buildTreeHelper(matrix, predictors, outcome_column); 
	}
};

#endif //!DECISION_TREE_H