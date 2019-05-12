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
private: 
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

		for (int i = 0; i < a[0].size(); i++)
		{
			for (int j = 0; j < b.size(); j++)
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
		// to encode 4 bits using the Hamming Algorithm, 
		// multiply the 4-bit sequence by the generation matrix.

		// then, modulo all numbers in the matrix by 2. 
	}
public:
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
				// char into two 4-bit sequences: a high and a low 
				pair<short, short> bits = splitChar(ch); 

				// encode high and low using the Hamming algorithm, 
				// with empty bit as first bit. 
				short highEncoded = encodeNibble(bits.first); 
			}
		}
	}

	void decodeFile(string file_name)
	{
	}
};

#endif //!HAMMINGCODES_H