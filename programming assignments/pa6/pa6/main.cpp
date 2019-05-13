/*
Assignment: PA #6 - Hamming Codes 
Description: This program encodes and decodes Hamming Codes from files given by
	the user. 
Author: Alex Childers
HSU ID: 013145324
Completion Time: 3.75 + 18:00 - 
In completing this program, I received help from the following people:
	Sanfoundry.com - used to fix matrix multiplication function: 
		https://www.sanfoundry.com/cpp-program-perform-matrix-multiplication/
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include "StringSplitter.h"
#include "HammingCodes.h"

using namespace std;

/*
Reflection suggestion: make it clear which way to rotate the parity vector 
*/

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

To decode a file:
- Open an input file with codes: [rootname].[ext].coded
- Remove the suffix(es) from the file name. Call this [rootname]
- Open an output file, [rootname].decoded.[ext]
- Asked user to decode? = false.
- Should you decode? = false.
- For every two bytes (high + low) in the input file:
	- For each byte:
		- Check its error status.
			- Convert byte to byte matrix.
			- Multiply parity check matrix by byte matrix to get parity vector.
			- Convert parity vector to binary number.
			- If parity != 0:
				-If "asked user to decode?" = false:
					- Tell user that file is corrupted.
					- Ask user if they'd like to decode, with the acknowledgement that
						the decode may be incorrect.
					- Set "asked user to decode?" = true.
						- If yes, do decode:
						- Set "should you decode?" = true.
			- If "should you decode?" = true:
				- Row [parity] of the received byte has been compromised.
				- Flip the bit of the [parity]th element of the byte matrix.
		- Extract data bits from byte matrix-- indices 2, 4, 5, 6.
		- Convert data bits --> short --> char. 
	- Combine 2 bytes of char into single byte of char. 
	- Output char to output file.

*/

/* TO DO 
- 
*/

// for the purpose of creating a "decoded" file name-- which pieces 
	// of the file name are where when split on "."? 
const int FILENAME_INDEX = 0;
const int EXT_INDEX = 1;

int main(int argc, char* argv[])
{
	HammingCodes coder{FILENAME_INDEX, EXT_INDEX}; 

   if (argc != 3)
   {
      cout << "Expected format: PA6.exe <encode / decode> <file_name>" << endl;
      return 0;
   }

   if (string{ argv[1] } == "encode")
   {
      //encode ABC.txt to ABC.txt.coded
      coder.encodeFile(string{ argv[2] });

      cout << "File encoded." << endl;
   }
   else if (string{ argv[1] } == "decode")
   {
      //decode ABC.txt.coded to ABC.decoded.txt
      coder.decodeFile(string{ argv[2] });
      cout << "File decoded." << endl;
   }
   else
   {
      cout << "Unexpected command." << endl;
   }

   return 0; 
}