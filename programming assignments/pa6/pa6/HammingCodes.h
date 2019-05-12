
// multiply() provided by Adam Carter; slight modifications based on 
// code by Sanfoundry.com 

#pragma once

#ifndef HAMMINGCODES_H
#define HAMMINGCODES_H

#include <string>
#include <fstream>

/* OUTLINE
To encode a file:
- Open an input file.
- Open an output file, [filename].coded
- For each character "char" in the input file:
	- Split char into two 4-bit sequences: a high and a low
	- Encode the high using the Hamming function, with the empty bit
		as the FIRST bit.
	- Output this encoding to [filename].coded
	- Encode the low using the Hamming function, with the empty bit as the
		FIRST bit as well.
	- Output this encoding to [filename].coded

*/

class HammingCodes
{
//DEBUG 
public:
//private: 
	vector<vector<short>> _generation_matrix{
		{ 1, 1, 0, 1 },
		{ 1, 0, 1, 1 },
		{ 1, 0, 0, 0 },
		{ 0, 1, 1, 1 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 },
	};

	// expects an 8-bit char; splits the char into two 4-bit 
	// sections (high and low) and returns the two segments 
	pair<short, short> splitChar(char ch)
	{
		// 1111 & char yields only bits 1-4 of char 
		short low = (0b1111) & ch;

		// 1111 & char shifted right 4 bits yields bits 5-8 of char
		short high = (0b1111) & (ch >> 4);
		return make_pair(high, low);
	}

	// expects two 2D vectors representing matrices; returns 
	// their product 
	template <typename T>
	vector<vector<T>> multiply(vector<vector<T>>& a, vector<vector<T>>& b)
	{
		//ensure proper matrix dimensions
		vector<vector<T>> product{};
		if (a.size() == 0 || a[0].size() != b.size())
		{
			return product;
		}

		//resize result matrix
		product.resize(a.size());
		for (auto& row : product)
		{
			row.resize(b[0].size());
		}

		for (int i = 0; i < a.size(); i++)
		{
			for (int j = 0; j < b[0].size(); j++)
			{
				for (int k = 0; k < b.size(); k++)
				{
					product[i][j] += a[i][k] * b[k][j];
				}
			}
		}
		return product;
	}

	// expects a nibble (half-byte AKA 4 bits); returns a 
	// byte representing the encoded nibble 
	short encodeNibble(short nibble)
	{
		// convert the nibble into a vector for matrix multiplication 
		vector<vector<short>> nibble_matrix{}; 
		for (int i = 3; i >= 0; i--)
		{
			// push digits of nibble into matrix  
			short next_digit = (0b0001) & (nibble >> i); 

			// since this is a 2D vector, push back a *list* 
			// containing 1 element 
			nibble_matrix.push_back({ next_digit });
		}

		// to encode 4 bits using the Hamming Algorithm, 
		// multiply the 4-bit sequence by the generation matrix.
		vector<vector<short>> result = multiply(_generation_matrix, nibble_matrix); 

		// modulo all numbers in vector by 2 and add to return number
		for (int i = 0; i < result.size(); i++)
		{
			result[i][0] = result[i][0] % 2; 
		}

		// convert vector of shorts back to short 
		short encoded = 0;
		for (int i = 0; i < result.size(); i++)
		{
			// shift encoded left by 1; then 'or' with the next digit of 
			// result to insert it in encoded  
			encoded = encoded << 1;
			short to_compare = result[i][0]; 
			encoded = encoded | to_compare; 
		}
		return encoded; 
	}
//public:
	// expects a file name; encodes the file using Hamming Codes 
	// and outputs the encoded file to [file_name].coded 
	void encodeFile(string file_name)
	{
		// open input file 
		ifstream in_stream{ file_name };
		if (in_stream.is_open() == true)
		{
			// open output file 
			ofstream out_stream{ file_name + ".coded" }; 
			while (in_stream.good() == true)
			{
				char ch = '\0';
				in_stream.get(ch);

				// for each character char in the input file, split 
				// char into two 4-bit sequences: pair<high, low> 
				pair<short, short> bits = splitChar(ch); 

				// encode high and low using the Hamming algorithm, 
				// with empty bit preceding all others 
				// (e.g. 100101 --> 0100101 by default) 
				short highEncoded = encodeNibble(bits.first); 
				short lowEncoded = encodeNibble(bits.second); 
			}
		}
	}

	void decodeFile(string file_name)
	{
	}
};

#endif //!HAMMINGCODES_H