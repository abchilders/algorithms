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

// This file mostly written by Alex--
// multiply() provided by Adam Carter; slight modifications based on 
// code by Sanfoundry.com

#include "HammingCodes.h" 

// expects an 8-bit char; splits the char into two 4-bit 
// sections (high and low) and returns the two segments 
pair<short, short> HammingCodes::splitChar(char ch)
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
vector<vector<T>> HammingCodes::multiply(vector<vector<T>>& a, vector<vector<T>>& b)
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

// expects a short, its bit length, and a reference to an empty 2D vector. 
// converts the short into a 2D vector of dimensions [length] x 1 
void HammingCodes::shortToMatrix(short number, int length, vector<vector<short>>& matrix)
{
	// we want to shift through number's digits length - 1 times 
	for (int i = length - 1; i >= 0; i--)
	{
		// push digits of short into matrix 
		short next_digit = (0b1) & (number >> i);

		// since this is a 2D vector, push back a *list* containing 1 
		// element 
		matrix.push_back({ next_digit });
	}
	return;
}

// expects an n x 1 vector; modulos all contents by 2
void HammingCodes::moduloBy2(vector<vector<short>>& matrix)
{
	for (unsigned int i = 0; i < matrix.size(); i++)
	{
		matrix[i][0] = matrix[i][0] % 2;
	}
}

// expects an n x 1 matrix that is the result of multiplying by a 
// generation or parity check matrix; converts this series of digits to a
// binary short 
short HammingCodes::matrixToShort(vector<vector<short>>& matrix)
{
	// modulo all numbers in vector by 2 and add to return number
	moduloBy2(matrix);

	// convert list of 1s and 0s to binary short
	short converted = 0;
	for (unsigned int i = 0; i < matrix.size(); i++)
	{
		// shift encoded left by 1; then 'or' with the next digit of 
		// result to insert it in encoded  
		converted = converted << 1;
		short to_compare = matrix[i][0];
		converted = converted | to_compare;
	}
	return converted;
}

// expects a nibble (half-byte AKA 4 bits); returns a 
// byte representing the encoded nibble 
short HammingCodes::encodeNibble(short nibble)
{
	// convert the nibble into a vector for matrix multiplication 
	vector<vector<short>> nibble_matrix{};
	shortToMatrix(nibble, 4, nibble_matrix);

	// to encode 4 bits using the Hamming Algorithm, 
	// multiply the 4-bit sequence by the generation matrix.
	vector<vector<short>> result = multiply(_generation_matrix, nibble_matrix);

	short encoded = matrixToShort(result);
	return encoded;
}

// expects a file name in the form of [name].[ext].[coded]. returns an output
// file name in the form [name].decoded.[txt]
string HammingCodes::decodedFileName(string file_name)
{
	vector<string> name_parts = StringSplitter::split(file_name, ".");
	return name_parts[_filename_index] + ".decoded." + name_parts[_ext_index];
}

// expects an encoded byte; returns the result of a parity check on this byte
short HammingCodes::checkForErrors(vector<vector<short>>& byte_matrix)
{
	// multiply parity check matrix by byte matrix to get parity vector 
	vector<vector<short>> parity_vector = multiply(
		_parity_check_matrix, byte_matrix);

	// modulo the parity vector by 2
	moduloBy2(parity_vector);

	// convert parity vector to binary number 
	reverse(parity_vector.begin(), parity_vector.end());
	short parity = matrixToShort(parity_vector);

	//DEBUG 
	return parity;
}

// expects a matrix of size n x 1 containing 0s and 1s, and a number 
// representing which bit to flip (from 1 to matrix size). 
// flips the requested bit 
void HammingCodes::correctMatrix(vector<vector<short>>& byte_matrix, short bit_to_flip)
{
	// convert bit to flip to an index 
	short flip_index = bit_to_flip - 1;

	// error checking 
	if ((bit_to_flip < 1) || (bit_to_flip > byte_matrix.size()))
	{
		return;
	}

	// flip 0 to 1 
	else if (byte_matrix[flip_index][0] == 0)
	{
		byte_matrix[flip_index][0] = 1;
	}

	// flip 1 to 0
	else
	{
		byte_matrix[flip_index][0] = 0;
	}
	return;
}

// expects nothing, returns a bool that is true if user wants
// the program to try decoding corrupted bits, and false 
// otherwise 
bool HammingCodes::askUserToDecode()
{
	char decode = '\0';
	cout << "The file has been corrupted. Would you like to "
		<< "attempt to decode the file? The results may be "
		<< "incorrect." << endl;
	cout << "Please enter: (y/n) ";
	cin >> decode;

	while (decode != 'y' && decode != 'n')
	{
		cout << "Invalid response. Please type y or n: ";
		cin >> decode;
	}

	if (decode == 'y')
	{
		return true;
	}
	else
	{
		return false;
	}
}

// expects two sets of bytes, high bits and low bits. 
// condenses them into a single character-byte
char HammingCodes::condenseBytes(char high, char low)
{
	char converted = high << 4;
	converted = converted | low;
	return converted; 
}

// expects an encoded byte, returns the decoded byte 
char HammingCodes::decodeByte(char byte, bool& asked_user, bool& try_decode)
{
	// convert byte to byte matrix 
	vector<vector<short>> byte_matrix{};
	shortToMatrix(byte, 7, byte_matrix);

	// check the error status of the byte 
	short error_status = checkForErrors(byte_matrix);

	// if error status is non-zero, there's an error. 
	if (error_status != 0)
	{
		// ask the user if we should decode 
		if (asked_user == false)
		{
			try_decode = askUserToDecode();
			asked_user = true;
		}

		// if the user wants us to decode, attempt to do so 
		if (try_decode == true)
		{
			// flip the bit of the [parity]th element of the byte matrix 
			correctMatrix(byte_matrix, error_status);
		}
	}

	// extract data bits from byte matrix
	vector<vector<short>> data_bits{};
	for (auto index : _data_indices)
	{
		data_bits.push_back({ byte_matrix[index][0] });
	}

	// convert data bits back to short/char bits 
	char result = matrixToShort(data_bits);
	return result;
}

// default constructor
HammingCodes::HammingCodes()
{
	_ext_index = 0;
	_ext_index = 1;
}

// constructor 
HammingCodes::HammingCodes(int name_index, int ext_index)
{
	_filename_index = name_index;
	_ext_index = ext_index;
}

// expects a file name; encodes the file using Hamming Codes 
// and outputs the encoded file to [file_name].coded 
void HammingCodes::encodeFile(string file_name)
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
			char highEncoded = encodeNibble(bits.first);
			char lowEncoded = encodeNibble(bits.second);

			// output this encoding 
			out_stream << highEncoded << lowEncoded;
		}
		out_stream.close();
	}
	in_stream.close();
	return;
}

// expects a file name; decodes the file using Hamming Codes 
// and outputs the decoded file to [file_name].decoded.[ext]
void HammingCodes::decodeFile(string file_name)
{
	// open input file 
	ifstream in_stream{ file_name };
	if (in_stream.is_open() == true)
	{
		// get output file name 
		string out_file = decodedFileName(file_name);
		ofstream out_stream{ out_file };

		// have we already asked the user if they want to 
		// try decoding errors?
		bool asked_user = false;

		// does the user want us to decode?
		bool decode_errors = false;
		while (in_stream.good() == true)
		{
			// get pair of high and low bytes 
			char high = '\0';
			char low = '\0';
			in_stream >> high;
			in_stream >> low;

			// translate each byte, decoding as needed 
			high = decodeByte(high, asked_user, decode_errors);
			low = decodeByte(low, asked_user, decode_errors);

			// combine high and low bits into one single char 
			char converted = condenseBytes(high, low); 
			out_stream << converted;
		}
		out_stream.close();
	}
	in_stream.close();
	return;
}