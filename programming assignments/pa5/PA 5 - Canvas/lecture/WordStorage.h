/*
Assignment: PA #5 - Levenshtein Edit Distance
Description: This program suggests corrections for misspelled words of an
	input file, and outputs the corrected text to an output file.
Author: Alex Childers
HSU ID: 013145324
Completion time: 11.75 hours
In completing this program, I received help from the following people:
	N/A
*/

// calculateEditDistance() is provided by Adam Carter; the rest of the 
// member functions are written by Alex. 

#pragma once
#ifndef WORDSTORAGE_H
#define WORDSTORAGE_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string> 
#include <vector>
#include "StringSplitter.h"

using namespace std; 

class WordStorage
{
private:
	vector<string> _dictionary{}; 
 
public:
	// calculates the edit distance between two words 
	int calculateEditDistance(const string& first, const string& second);

	// read in the dictionary file to _dictionary
	void readDictionary(const string dict_file); 

	// computes the 10 most probable suggestions for a word based
	// on edit distance
	void computeTopTenWords(
		const string& next_word,
		const string& corrections_file,
		vector<pair<string, int>>& corrected_words); 

	// records the correct spelling of a word as given by the user
	string recordCorrectSpelling(
		const string& next_word, 
		const string& context_line, 
		vector<pair<string, int>>& corrected_words); 

	// outputs autocorrect results for a word to a file 
	void outputAutoCorrects(
		const string& corrections_file, 
		const vector<pair<string, int>>& corrected_words); 

	// returns true if next_word exists in the dictionary
	bool wordExistsInDict(const string& next_word); 

	// removes punctation marks from the beginning and end of word;
	// returns each set of punctuation marks 
	pair<string, string> removePuncts(string& word);
};

class PairComparer
{
public:
	bool operator()(pair<string, int> first, pair<string, int> second)
	{
		return first.second > second.second;
	}
};

#endif // ! WORDSTORAGE_H 