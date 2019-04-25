/* 
Assignment: PA #5 - Levenshtein Edit Distance
Description: This suggests corrections for misspelled words of an input file, 
	and outputs the corrected text to an output file.
Author: Alex Childers 
HSU ID: 013145324
Completion time: 9 hours
In completing this program, I received help from the following people:
	N/A
*/

// Note: this program currently only autocorrects all lowercase words.
// You can use it to autocorrect words with capitalization; they just 
// won't be in the dictionary.

// RE-ADD WORDSTORAGE.H WHEN I'M READY 

/* To do:
	-Abstraction! Re-add WordStorage.h and abstract out as needed.
	-Review PA5.docx to make sure I've met all specifications.
	-Write PA 5 reflection. 
	-Clean up code, and refactor if time.
*/

#include <queue>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_map> 
#include "StringSplitter.h"
#include "WordStorage.h"

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

	// if there are no chars left in word after removing punctuation,
	// the whole "word" is just punctuation. stop here 
	if (word == "")
	{
		return make_pair(str_start, ""); 
	}

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
	// object containing methods to handle autocorrecting words 
	WordStorage correct_words{}; 

	//contains our dictionary 
	vector<string> dictionary = correct_words.readDictionary(DICTIONARY); 

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
				pair<string, string> punct = removePuncts(next_word);

				// is the entire "word" just punctuation?
				if (next_word == "")
				{
					out_stream << punct.first << " ";
					continue;
				}

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
					/*
					vector<pair<string, int>> corrected_words{};

					// word is misspelled. do we already have a list of 
					// autocorrects for it?
					string corrections_file = next_word + FILE_SUFFIX;
					ifstream autocorrects{ corrections_file };
					if (autocorrects.is_open() == true)
					{
						while (autocorrects.good() == true)
						{
							// get each line and place into vector
							string word_line = "";
							getline(autocorrects, word_line); 
							if (word_line != "" && word_line != " ")
							{
								vector<string> line = StringSplitter::split(word_line, " ");

								// just in case we have space-separated compound words
								// (e.g. real estate), only separate the last word
								// representing edit distance from the rest of the line 
								ostringstream file_word{};
								for (int i = 0; i < line.size() - 2; i++)
								{
									file_word << line[i] << " ";
								}
								file_word << line[line.size() - 2];
								int distance = stoi(line[line.size() - 1]);

								corrected_words.push_back(make_pair
								(file_word.str(), distance));
							}
						}
					}
					else
					{
						// compute edit distance for all
						// words in dictionary 
						priority_queue<pair<string, int>,
							vector<pair<string, int>>,
							PairComparer> edit_distances{};

						for (auto dict_word : dictionary)
						{
							int distance = correct_words.calculateEditDistance(next_word, dict_word);
							edit_distances.push(make_pair(dict_word, distance));
						}

						// put 10 words with lowest edit distances into vector
						int counter = 0;
						while (counter < 10)
						{
							pair<string, int> next_lowest = edit_distances.top();

							// does this word already exist in vector?
							auto found = find(corrected_words.begin(),
								corrected_words.end(),
								next_lowest);
							if (found == corrected_words.end())
							{
								// if not, add it 
								corrected_words.push_back(next_lowest);
								counter++;
							}
							edit_distances.pop();
						}
					}
					autocorrects.close();
					*/

					vector<pair<string, int>>* corrected_words = correct_words.computeTopTenWords(
						next_word, FILE_SUFFIX, dictionary); 

					// display the 10 most probable answers. let the user pick the 
					// correct one or specify their own 
					cout << "Unknown word: " << next_word << endl;
					cout << "\t Context: " << context_line << endl;
					cout << "Corrected word: " << endl;
					cout << "1. None of the words below are correct" << endl;
					int numbering = 2;
					for (auto word_pair : corrected_words)
					{
						string word = word_pair.first;
						cout << numbering << ". " << word << endl;
						numbering++;
					}

					string choice_str = "";
					cout << "Enter selection: ";
					getline(cin, choice_str);
					int choice = stoi(choice_str);

					// make sure input is valid 
					while (choice > corrected_words.size() + 1)
					{
						cout << "Enter selection: ";
						getline(cin, choice_str);
						choice = stoi(choice_str);
					}

					string right_word = "";
					if (choice == 1)
					{
						// record user-defined word 
						cout << "Enter corrected word: ";
						getline(cin, right_word);
						pair<string, int> new_word = make_pair(right_word, 0);
						corrected_words.insert(corrected_words.begin(), new_word);
					}
					else
					{
						pair<string, int> best_word = corrected_words[choice - 2];
						right_word = best_word.first;
						corrected_words.erase(corrected_words.begin() + (choice - 2));
						corrected_words.insert(corrected_words.begin(), best_word);
					}

					// output corrected word to file 
					out_stream << punct.first << right_word << punct.second << " ";

					// output list of autocorrects to file 
					ofstream new_autocorrects{ corrections_file };
					if (new_autocorrects.is_open() == true)
					{
						for (auto word_pair : corrected_words)
						{
							new_autocorrects << word_pair.first << " "
								<< word_pair.second << endl;
						}
					}
					new_autocorrects.close();
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