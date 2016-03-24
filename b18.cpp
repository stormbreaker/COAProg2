/*
Name: Program 2 - FPGA Simulator
Author: Benjamin Kaiser
Class: CSC 317 Computer Organization
Professor: Dr. Christer Karlsson
Due Date: March 23, 2016
Known Bugs: No known Bugs
Description:  This is a simulator of a FPGA developed for Dr. Karlsson for the
	second program of the CSC 317 class.  It has default constraints specified as
	constants.  The J constant sets the number of original inputs the gate array
	can handle.  The K constant sets the number of outputs the gate array can
	handle.  The M constant sets the number of NAND gates are in a single column
	of the gate array matrix.  The N constant handles the number of columns in the
	gate array.
Compile: use the accompanying Makefile or alternatively use the following command:
	g++ b18.cpp -o b18
Run: ./b18 <inputfile>
	./b18 example
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cmath>
#include <bitset>
#include <string>

using namespace std;

bool nand(bool in1, bool in2);

//compile-time parameters
//default parameters
const int J = 8;
const int K = 4;
const int M = 10;
const int N = 10;

//parameters used to run the sample input from the program description
/*
const int J = 4;
const int K = 2;
const int M = 3;
const int N = 2;
*/

const int LENGTH = pow(2, J);

/*
Name: main
Description:  This function is basically the entire simulator.  It handles the
	file input, the output to the screen and calculating the signals throughout
	the entire array.  It uses two main arrays blackIn and redOut.  These store
	the boolean values of the red and black wires as they enter the circuit and
	go through the circuit.  It also uses a vector of strings which the input
	pattern is sent into after the counter is run through the bitset.  This
	is done because the input patterns appear to be binary with each input being
	a different bit.  These count up from 0 to (2^j) - 1.  The code has to count
	the number of j's and k's actually used to print out the inputs and outputs.
	After that, it simply loops through the two arrays and calculates the values
	in each array.
Author: Benjamin Kaiser
*/
int main(int argc, char** argv)
{

	int wirelist[(2 * M * N) + K]; //the
	bool redOut[(M * N) + J]; //output from each gate and also the original inputs
	bool blackIn[(2 * M * N)]; //input into each nand gate and also the final outputs

	vector<string> truthTable(LENGTH); //intial inputs iterated through

	bool jUsed[J]; //keep track of which j inputs are actually used
	bool kUsed[K]; //keep track of which k inputs are actually used

	int jCols[J]; //keeps track of which j inputs need to be printed
	int kCols[K]; //keeps track of which k inputs need to be printed

	int tempRed, tempBlack; //temporary input variables to read the wiring list
	int actualUsed = 0; //count the number of j inputs that are actually used

	int kActualUsed = 0; //count the number of k inputs that are actually used

	ifstream fin; //file open stream to read the wire input

	//open the wiring list
	fin.open(argv[1]);

	//initialize jCols to -1
	for (int i = 0; i < J; i++)
	{
		jCols[i] = -1;
	}

	//initialize kCols to -1
	for (int i = 0; i < K; i++)
	{
		kCols[i] = -1;
	}

	// initialize wirelist to all -1 (unused)
	for (int i = 0; i < (2 * M * N) + K; i++)
	{
		wirelist[i] = -1;
	}

	//initialize jUsed to false
	for (int i = 0; i < J; i++)
	{
		jUsed[i] = false;
	}

	//initialize kUsed to false
	for (int i = 0; i < K; i++)
	{
		kUsed[i] = false;
	}

	//initialize blackIn to false
	for (int i = 0; i < (2 * M * N); i++)
	{
		blackIn[i] = false;
	}

	int index = 0; //used to put the j and k wires into the proper columns for output

	//store the wiring connections
	while (fin >> tempRed && fin >> tempBlack)
	{
		wirelist[tempBlack] = tempRed;
		if (tempRed < J)
		{
			jUsed[tempRed] = true;
		}
		if (tempBlack >= (2 * M * N))
		{
			kUsed[tempBlack - (2 * M * N)] = true;
		}
	}

	//close wiring list
	fin.close();

	//COUNT THE NUMBER OF J'S ACTUALLY USED!!!!!
	for (int i = 0; i < J; i++)
	{
		if (jUsed[i] == true)
		{
			actualUsed++;
			jCols[index] = i;
			index++;
		}
	}

	index = 0; //reset to count the k's actually used

	//COUNT THE NUMBER OF K's ACTUALLY USED!
	for (int i = 0; i < K; i++)
	{
		if (kUsed[i] == true)
		{
			kActualUsed++;
			kCols[index] = i + (2 * M * N);
			index++;
		}
	}

	// fill the truth table (initial values)
	for (int i = 0; i < pow(2, actualUsed); i++)
	{
		bitset<J> tempTable(i);
		tempTable <<= (J - actualUsed);
		truthTable[i] = tempTable.to_string();
		//cout << truthTable[i] << endl;
	}

	//output
	for (int i = 0; i < (actualUsed + kActualUsed) * 5; i++)
	{
		//border
		cout << "-";
	}
	cout << endl;
	for (int i = 0; i < actualUsed; i++)
	{
		//input labels
		cout << "I" << jCols[i] << " | ";
	}
	//cout << "kactual " << kActualUsed << endl;
	for (int i = 0; i < kActualUsed; i++)
	{
		//output labels
		cout << "O" << i << " | ";
	}
	cout << endl;
	for (int i = 0; i < (actualUsed + kActualUsed) * 5; i++)
	{
		//bottom border
		cout << "-";
	}
	cout << endl;
	//end output

	//entering the main computational for loop which loops down the rows
	//of the truth table
	for (int bigIndex = 0; bigIndex < pow(2, actualUsed); bigIndex++)
	{
		for (int i = 0; i < actualUsed; i++)
		{
			redOut[jCols[i]] = (truthTable[bigIndex][i] == '1'); //setting up the original jVals
		}
		for (int i = 0; i < (2 * M * N); i +=2)
		{
			if (wirelist[i] != -1)
			{
				//find out what the values for the inputs to the nand gate are
				blackIn[i] = (redOut[wirelist[i]] == true);
				blackIn[i+1] = (redOut[wirelist[i+1]] == true);
				//nand the two values together and save to redOut
				redOut[i/2 + J] = nand(blackIn[i], blackIn[i+1]);
			}
		}

		//final outputs
		for (int i = (2 * M * N); i < (2 * M * N) + K; i++)
		{
			if (wirelist[i] != -1)
			{
				//connect the final nand gates to the final outputs
				blackIn[i] = (redOut[wirelist[i]] == true);
			}
		}

		//output values to screen
		for (int i = 0; i < actualUsed; i++)
		{
			cout << truthTable[bigIndex][i] << "  | ";
		}
		for (int i = (2 * M * N); i < (2 * M * N) + K; i++)
		{
			if (kUsed[i - (2 * M * N)] == true)
			{
				cout << blackIn[i] << "  | ";
			}
		}
		cout << endl;
		//end output

	}

	//end program
	return 0;
}

/*
Name: nand
Description: This function takes two booleans together and performs a nand operation
	by taking the two bools, nanding them and returning the complement.
Author: Benjamin Kaiser
Parameters:
	in1: This is the first boolean input to run through a NAND gate.
	in2: This is the second boolean input to run through a NAND gate.
Returns:
	in1 and in2 ANDed together and inverted.
*/
bool nand(bool in1, bool in2)
{
	//do a boolean and and return the complement of it
	return !(in1 && in2);
}
