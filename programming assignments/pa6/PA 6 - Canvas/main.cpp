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

// This file is written by Adam Carter. 

#include <cstdlib>
#include <sstream>
#include "HammingCodes.h"

// for the purpose of creating a "decoded" file name-- which pieces 
// of the file name are at which index, when split on "."? 
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