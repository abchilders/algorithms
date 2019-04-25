#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "WordStorage.h"

int WordStorage::calculateEditDistance(const string& first, const string& second)
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

pair<string, string> WordStorage::removePuncts(string& word)
{
	ostringstream begin_punct{};
	bool punct = ispunct(word[0]);
	while (ispunct(word[0]))
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
	while (ispunct(word[word.length() - 1]))
	{
		end_punct << word[word.length() - 1];
		word.erase(word.end() - 1);
	}
	string str_end = end_punct.str();
	reverse(str_end.begin(), str_end.end());

	return make_pair(str_start, str_end);
}

void WordStorage::readDictionary(string dict_file)
{
	// read the words.txt dictionary into the program 
	ifstream words{ dict_file };
	if (words.is_open() == true)
	{
		while (words.good() == true)
		{
			// read dictionary words into vector
			string next_word = "";
			words >> next_word;
			_dictionary.push_back(next_word);
		}
	}
	words.close();
	return;
}

// computes the 10 most probable suggestions for a word based
// on edit distance
vector<pair<string, int>>* WordStorage::computeTopTenWords(
	string& next_word,
	string& corrections_file,
	vector<pair<string, int>>& corrected_words)
{
	// word is misspelled. do we already have a list of 
	// autocorrects for it?
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

		for (auto dict_word : _dictionary)
		{
			int distance = calculateEditDistance(next_word, dict_word);
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
	return;
}

// records the correct spelling of a word as given by the user
string WordStorage::recordCorrectSpelling(string& next_word, string& context_line,
	vector<pair<string, int>>& corrected_words)
{
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
	return right_word; 
}

// outputs autocorrect results for a word to a file 
void WordStorage::outputAutoCorrects(string& corrections_file,
	vector<pair<string, int>>& corrected_words)
{
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
	return; 
}

// returns true if next_word exists in the dictionary
bool WordStorage::wordExistsInDict(string& next_word)
{
	auto found = find(_dictionary.begin(), _dictionary.end(), next_word);
	if (found == _dictionary.end())
	{
		return false; 
	}
	else
	{
		return true; 
	}
}