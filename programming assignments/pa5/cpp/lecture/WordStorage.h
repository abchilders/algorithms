#pragma once
#ifndef WORDSTORAGE_H
#define WORDSTORAGE_H

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

	// removes punctation marks from the beginning and end of word;
	// returns each set of punctuation marks 
	pair<string, string> removePuncts(string& word); 
public:
	// calculates the edit distance between two words 
	int calculateEditDistance(const string& first, const string& second);

	// read in the dictionary file to _dictionary
	void readDictionary(string dict_file); 

	// opens input/output streams to files given by user and reads in 
	// the user's input file 
	void readUserFile(); 

	// computes the 10 most probable suggestions for a word based
	// on edit distance
	vector<pair<string, int>>* computeTopTenWords(
		string& next_word,
		string correct_file_suffix); 

	// records the correct spelling of a word as given by the user
	void recordCorrectSpelling(); 

	// outputs autocorrect results for a word to a file 
	void outputAutoCorrects(); 

	// outputs corrected text to the user-specified output file 
	void outputCorrectedText(); 
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