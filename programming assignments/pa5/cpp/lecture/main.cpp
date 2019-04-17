/* 
Assignment: PA #5 - Levenshtein Edit Distance
Description: This suggests corrections for misspelled words of an input file, 
	and outputs the corrected text to an output file.
Author: Alex Childers 
HSU ID: 013145324
Completion time: 1.25 + 15:00 - 
In completing this program, I received help from the following people:
	N/A
*/

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

// to use ispunct() function 
#include <cctype> 

using namespace std;

const string DICTIONARY = "words.txt"; 

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
			string next_word = ""; 
			in_stream >> next_word;  
			
			/* Punctuation rules:
				- Remove any non-apostrophe punctuation marks from the 
					front and back. 
				- If there is an apostrophe at both the back and 
						the front of the string, remove them.
						e.g: 'hello', 'oy'
						Note that this won't work for a contraction like
						'n', but IMO, that's an exceptional case so whatever.
				- Otherwise, the punctuation is part of the word. 
						Keep it. 
						e.g: 'twas, she's, y'all'dn't've'f'i'd'nt, 
							back-to-back, string.length()
						(since I'm too lazy to code exceptions for 
						hyphenated words, we'll treat compound words as one 
						word) 
				- BUT WHAT IF THERE'S MULTIPLE PUNCTUATION CHARACTERS NEXT
					TO EACH OTHER? e.g. (), ..., !!!
			*/

			// remove punctuation according to above rules 
			unordered_map<int, char> removed_puncts{}; 
			for (int i = 0; i < next_word.length(); i++)
			{
				if (ispunct(next_word[i]))
				{
					// remove all punctuation marks from front and back,
					// IF they're not apostrophes 
					if (i == 0 && next_word[i] != '\'')
					{
						removed_puncts[i] = next_word[i]; 
					}
					else if ((i == next_word.length() - 1)
						&& next_word[i] != '\'')
					{
						removed_puncts[i] = next_word[i]; 
					}
					// what if the first character IS an apostrophe? 
					// then remember to remove them both 
					else if (i == 0 && next_word[i] == '\''
						&& next_word[next_word.length() - 1] == '\'')
					{
						removed_puncts[i] = next_word[i]; 
						removed_puncts[next_word.length() - 1] =
							next_word[next_word.length() - 1];
					}
					// or, if we've in the middle of the word and the 
					// punctuation is not an apostrophe, remove the 
					// punctuation and remember to correct the pieces
					// of the word before and after the punctuation 

					// TO DO: IMPLEMENT THAT LAST CASE OF PUNCTUATION RULES 
				}

			}


			// is the next word misspelled? check if it exists in 
			// dictionary. if not, we need to compute edit distances and 
			// correct before outputting to file 
			auto found = find(dictionary.begin(), dictionary.end(), next_word); 
			if (found != dictionary.end())
			{
				// word exists in dictionary, so output to file 
				out_stream << next_word << " "; 
			}
			else
			{
				// word is misspelled. compute edit distance for all
				// words in dictionary 
				
			}
		}
	}

	// test 
   cout << calculateEditDistance("dog", "frog") << endl;
   return 0;
}