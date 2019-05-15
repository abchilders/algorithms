/*
Assignment: PA #6 - Hamming Codes
Description: This program encodes and decodes Hamming Codes from files given by
	the user.
Author: Alex Childers
HSU ID: 013145324
Completion Time: 6.5 hours
In completing this program, I received help from the following people:
	Sanfoundry.com - used to fix matrix multiplication function:
		https://www.sanfoundry.com/cpp-program-perform-matrix-multiplication/
*/

// This file is written by Alex. 

#pragma once

#ifndef HAMMINGCODES_H
#define HAMMINGCODES_H

#include <string>
#include <fstream>
#include <iostream>
#include "StringSplitter.h"

using namespace std; 

class HammingCodes
{
private: 
	// for the purpose of creating a "decoded" file name-- which pieces 
	// of the file name are where when split on "."? 
	int _filename_index = 0; 
	int _ext_index = 1; 

	// used to encode bytes 
	vector<vector<short>> _generation_matrix {
		{ 1, 1, 0, 1 },
		{ 1, 0, 1, 1 },
		{ 1, 0, 0, 0 },
		{ 0, 1, 1, 1 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 },
	};

	// used to decode bytes 
	vector<vector<short>> _parity_check_matrix{
		{1, 0, 1, 0, 1, 0, 1},
		{0, 1, 1, 0, 0, 1, 1},
		{0, 0, 0, 1, 1, 1, 1}
	}; 

	// which indices of a byte matrix will the data bits be in?
	vector<int> _data_indices = { 2, 4, 5, 6 }; 

	// expects an 8-bit char; splits the char into two 4-bit 
	// sections (high and low) and returns the two segments 
	pair<short, short> splitChar(char ch);

	// expects two 2D vectors representing matrices; returns 
	// their product 
	template <typename T>
	vector<vector<T>> multiply(vector<vector<T>>& a, vector<vector<T>>& b);
	
	// expects a short, its bit length, and a reference to an empty 2D vector. 
	// converts the short into a 2D vector of dimensions [length] x 1 
	void shortToMatrix(short number, int length, vector<vector<short>>& matrix);
	
	// expects an n x 1 vector; modulos all contents by 2
	void moduloBy2(vector<vector<short>>& matrix);

	// expects an n x 1 matrix that is the result of multiplying by a 
	// generation or parity check matrix; converts this series of digits to a
	// binary short 
	short matrixToShort(vector<vector<short>>& matrix);

	// expects a nibble (half-byte AKA 4 bits); returns a 
	// byte representing the encoded nibble 
	short encodeNibble(short nibble);

	// expects a file name in the form of [name].[ext].[coded]. returns an output
	// file name in the form [name].decoded.[txt]
	string decodedFileName(string file_name);

	// expects an encoded byte; returns the result of a parity check on this byte
	short checkForErrors(vector<vector<short>>& byte_matrix);

	// expects a matrix of size n x 1 containing 0s and 1s, and a number 
	// representing which bit to flip (from 1 to matrix size). 
	// flips the requested bit 
	void correctMatrix(vector<vector<short>>& byte_matrix, short bit_to_flip);

	// expects nothing, returns a bool that is true if user wants
	// the program to try decoding corrupted bits, and false 
	// otherwise 
	bool askUserToDecode();

	// expects two sets of bytes, high bits and low bits. 
	// condenses them into a single character-byte
	char condenseBytes(char high, char low);

	// expects an encoded byte, returns the decoded byte 
	char decodeByte(char byte, bool& asked_user, bool& try_decode);

public:
	// default constructor
	HammingCodes();

	// constructor 
	HammingCodes(int name_index, int ext_index);

	// expects a file name; encodes the file using Hamming Codes 
	// and outputs the encoded file to [file_name].coded 
	void encodeFile(string file_name);

	// expects a file name; decodes the file using Hamming Codes 
	// and outputs the decoded file to [file_name].decoded.[ext]
	void decodeFile(string file_name);
};

#endif //!HAMMINGCODES_H