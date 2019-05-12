/*
Assignment: PA #6 - Hamming Codes 
Description: This program encodes and decodes Hamming Codes from files given by
	the user. 
Author: Alex Childers
HSU ID: 013145324
Completion Time: 14:00 - 
In completing this program, I received help from the following people:
	Sanfoundry.com - fixed matrix multiplication function: 
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

/* TO DO 
- uncomment "private" in HammingCodes.h
*/

int main(int argc, char* argv[])
{
	HammingCodes coder{}; 

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

   //DEBUG - testing ability to convert a character into two bytes
   pair<short, short> test = coder.splitChar('T'); 

   return 0; 
}