/* 
Assignment: PA #5 - Levenshtein Edit Distance
Description: This suggests corrections for misspelled words of an input file, 
	and outputs the corrected text to an output file.
Author: Alex Childers 
HSU ID: 013145324
Completion time: 11:45 + 
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
*/

#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
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
	unordered_map<string, int> dictionary{}; 

	// read the words.txt dictionary into the program 
	ifstream words{ DICTIONARY }; 
	if (words.is_open() == true)
	{
		while (words.good() == true)
		{
			// read dictionary words into hashtable, for O(1) access
			string next_word = "";
			words >> next_word; 
			dictionary[next_word]++; 
		}
	}
	words.close();

	// prompt the user for an input file to autocorrect and a destination 
	// to output the corrected text
	string input_file = ""; 
	string output_file = ""; 
	cout << "Enter input file: ";
	getline(cin, input_file); 
	cout << "Enter destination file: "; 
	getline(cin, output_file); 

	// open input file and check for misspelled words
	ifstream in_stream{ input_file }; 
	if (in_stream.is_open() == true)
	{
		while (in_stream.good() == true)
		{
			string next_word = ""; 

			// is the next word misspelled? check if it exists in 
			// dictionary 
			if (dictionary[next_word] < 1)
			{
				/* if it's misspelled, handle it accordingly and
					correct it 
				*/
			}

		}
	}

	// test 
   cout << calculateEditDistance("dog", "frog") << endl;
   return 0;
}