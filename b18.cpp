/*
Name:
Author:
Class:
Professor: Benjamin Kaiser
Due Date:
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

/*
const int J = 4;
const int K = 2;
const int M = 3;
const int N = 2;
*/

const int LENGTH = pow(2, J);
/*
Name:
Purpose:
Author:
*/
int main(int argc, char** argv)
{

	int wirelist[(2 * M * N) + K];
	bool redOut[(M * N) + J]; //my output from each gate
	bool blackIn[(2 * M * N)];

	vector<string> truthTable(LENGTH); //intial inputs iterated through

	bool jUsed[J];
	bool kUsed[K];

	int jCols[J];
	int kCols[K];

	int tempRed, tempBlack;
	int actualUsed = 0;

	int kActualUsed = 0;

	ifstream fin;
	fin.open(argv[1]);

	//kUsed indices

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

	int index = 0;

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

	index = 0;
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
		cout << "-";
	}
	cout << endl;

	for (int i = 0; i < actualUsed; i++)
	{
		cout << "I" << jCols[i] << " | ";
	}
	//cout << "kactual " << kActualUsed << endl;
	for (int i = 0; i < kActualUsed; i++)
	{
		cout << "O" << i << " | ";
	}
	cout << endl;
	for (int i = 0; i < (actualUsed + kActualUsed) * 5; i++)
	{
		cout << "-";
	}
	cout << endl;
	//end output

	for (int bigIndex = 0; bigIndex < pow(2, actualUsed); bigIndex++)
	{
		for (int i = 0; i < actualUsed; i++)
		{
			redOut[jCols[i]] = (truthTable[bigIndex][i] == '1'); // setting up the original jVals
			//for inputs
		}
		for (int i = 0; i < (2 * M * N); i +=2)
		{
			if (wirelist[i] != -1)
			{
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
				blackIn[i] = (redOut[wirelist[i]] == true);
			}
		}
		//output to screen
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
	}
	return 0;
}
/*
Name:
Purpose:
Author:
*/
bool nand(bool in1, bool in2)
{
	return !(in1 && in2);
}
