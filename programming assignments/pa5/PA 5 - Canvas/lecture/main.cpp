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

// Note: this program currently only autocorrects all lowercase words.
// You can use it to autocorrect words with capitalization; they just 
// won't be in the dictionary.

#include <queue>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_map> 
#include "StringSplitter.h"
#include "WordStorage.h"

// to use ispunct()  
#include <cctype> 

using namespace std;

// the name of the dictionary file 
const string DICTIONARY = "words.txt"; 

// the suffix of files holding autocorrect results for each word 
const string FILE_SUFFIX = ".dat"; 

int main(void)
{
	// object containing methods to handle autocorrecting words 
	WordStorage correct_words{}; 

	// read in the dictionary file 
	correct_words.readDictionary(DICTIONARY); 

	// prompt the user for an input file to autocorrect and a destination 
	// to output the corrected text
	string input_file = "";
	string output_file = "";
	cout << "Enter input file: ";
	getline(cin, input_file);
	cout << "Enter destination file: ";
	getline(cin, output_file);

	// open input and output streams 
	ifstream in_stream{ input_file };

	if (in_stream.is_open() == true)
	{
		ofstream out_stream{ output_file };

		// check for misspelled words
		while (in_stream.good() == true)
		{
			// remember the current line of text for context 
			string context_line = "";
			getline(in_stream, context_line);

			// consider each word in the current line
			vector<string> words = StringSplitter::split(context_line, " ");
			for (auto next_word : words)
			{
				// remove punctuation
				pair<string, string> punct = correct_words.removePuncts(next_word);

				// is the entire "word" just punctuation?
				if (next_word == "")
				{
					out_stream << punct.first << " ";
					continue;
				}

				// is the next word misspelled? check if it exists in 
				// dictionary. if not, we need to compute edit distances and 
				// correct before outputting to file 
				if (correct_words.wordExistsInDict(next_word) == true)
				{
					// word exists in dictionary, so output to file 
					out_stream << punct.first << next_word << punct.second << " ";
				}
				else
				{
					// compute top 10 autocorrects for the current word
					string corrections_file = next_word + FILE_SUFFIX; 
					vector<pair<string, int>> corrected_words{}; 
					correct_words.computeTopTenWords(next_word, corrections_file, corrected_words); 
					
					// determine the correct spelling of said word from user input 
					string right_word = correct_words.recordCorrectSpelling(next_word, context_line,
						corrected_words); 

					// output corrected word to file 
					out_stream << punct.first << right_word << punct.second << " ";

					// output list of autocorrects to file 
					correct_words.outputAutoCorrects(corrections_file, corrected_words); 
				}
			}
		}
		out_stream.close();
	}
	else
	{
		cout << "Failed to open file " << input_file << "." << endl;
	}
	in_stream.close();

	return 0;
}