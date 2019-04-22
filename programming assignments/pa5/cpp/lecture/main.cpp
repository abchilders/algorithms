/* 
Assignment: PA #5 - Levenshtein Edit Distance
Description: This suggests corrections for misspelled words of an input file, 
	and outputs the corrected text to an output file.
Author: Alex Childers 
HSU ID: 013145324
Completion time: 3 hours + 10:30 - 
In completing this program, I received help from the following people:
	N/A
*/

// RE-ADD WORDSTORAGE.H WHEN I'M READY 

/* To do:
3.) For each misspelled word:
	a.) Compute the 10 most probable suggestions based on edit distance.
		Also, provide an opportunity for the user to specify their own 
		autocorrected answer.
		*** make sure the program gracefully handles improper user input
	b.) Having obtained the correct spelling, place the correct spelling
		at the top of the list (such that, next time we see this misspelling,
		it's the first suggestion made by the autocorrect). 
	c.) Save the autocorrect results to a file (so that, when we 
		encounter the misspelling again, we don't have to recompute all of 
		the distances. 
4.) Once all the misspelled words have been corrected, write the corrected
	text to the user-specified output file. 

* Check for punctuation. Decide my rules for which punctuation to ELIMINATE
	and which to keep. 
	- I've typed punctuation rules below, implement them.
	- It's worth looking into string::insert and related string functions. 
*/

#include <queue>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_map> 
#include <sstream>
#include "StringSplitter.h"

// to use ispunct() function 
#include <cctype> 

using namespace std;

// the name of the dictionary file 
const string DICTIONARY = "words.txt"; 

// the suffix of files holding autocorrect results for each word 
const string FILE_SUFFIX = ".dat"; 

class PairComparer
{
public:
	bool operator()(pair<string, int> first, pair<string, int> second)
	{
		return first.second > second.second; 
	}
};

int calculateEditDistance(const string& first, const string& second)
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

// removes punctuation from the beginning and end of a string; returns the 
// punctuation that was removed from either end 
pair<string, string> removePuncts(string& word)
{
	ostringstream begin_punct{};
	bool punct = ispunct(word[0]); 
	while(ispunct(word[0]))
	{
		// record punctuation so that we can re-add it later 
		begin_punct << word[0];
		word.erase(word.begin());
	}
	string str_start = begin_punct.str();

	ostringstream end_punct{};
	while(ispunct(word[word.length() - 1]))
	{
		end_punct << word[word.length() - 1];
		word.erase(word.end() - 1); 
	}
	string str_end = end_punct.str(); 
	reverse(str_end.begin(), str_end.end());
	
	return make_pair(str_start, str_end); 
}

int main(void)
{
	//contains our dictionary 
	vector<string> dictionary{}; 

	// read the words.txt dictionary into the program 
	ifstream words{ DICTIONARY }; 
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
	ofstream out_stream{ output_file }; 

	if (in_stream.is_open() == true)
	{
		// check for misspelled words
		while (in_stream.good() == true)
		{
			// remember the current line of text for context 
			string context_line = ""; 
			getline(in_stream, context_line); 

			// consider each word in the current line
			vector<string> words = StringSplitter::split(context_line, " "); 
			for(auto next_word: words)
			{
				// remove punctuation
				pair<string, string> punct = removePuncts(next_word); 

				// is the next word misspelled? check if it exists in 
				// dictionary. if not, we need to compute edit distances and 
				// correct before outputting to file 
				auto found = find(dictionary.begin(), dictionary.end(), next_word); 
				if (found != dictionary.end())
				{
					// word exists in dictionary, so output to file 
					out_stream << punct.first << next_word << punct.second << " "; 
				}
				else
				{
					// word is misspelled. compute edit distance for all
					// words in dictionary 
					priority_queue<pair<string, int>,
						vector<pair<string, int>>,
						PairComparer> edit_distances{};

					for (auto dict_word : dictionary)
					{
						int distance = calculateEditDistance(next_word, dict_word);
						edit_distances.push(make_pair(dict_word, distance));
					}

					// now that we've calculated all edit distances, display the 
					// 10 most probable answers. let the user pick the correct one
					// or specify their own 
					cout << "Unknown word: " << next_word << endl; 
					cout << "\t Context: " << context_line << endl; 
					cout << "Corrected word: " << endl; 
					cout << "1. None of the words below are correct" << endl; 
					for (int i = 2; i <= 11; i++)
					{
						// pop the next closest word from the queue 
						string corrected = edit_distances.top().first; 
						edit_distances.pop(); 
						cout << i << ". " << corrected << endl; 
					}
				} 
			}
		}
	}

	// test 
	string hello = "...$hello??,,";
	pair<string, string> test = removePuncts(hello);
   cout << calculateEditDistance("dog", "frog") << endl;
   return 0;
}