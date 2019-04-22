#pragma once
#ifndef WORDSTORAGE_H
#define WORDSTORAGE_H

#include <string> 

using namespace std; 

class WordStorage
{
private:
	const string _DICTIONARY = "words.txt"; 

	// calculates the edit distance between two words 
	int calculateEditDistance(const string& first, const string& second);

public:
	// prompts the user to input where they want words stored 
	void getUserStorage();
};

#endif // ! WORDSTORAGE_H 