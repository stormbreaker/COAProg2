#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cmath>
#include <bitset>
#include <string>

using namespace std;

bool nand(bool in1, bool in2);
int mapBlackToRed(int blackNum);
int* mapRedToBlack(int redNum);
int getCol(int blackNum);
int getRow(int redNum);
int* map2Darray(int redNum);


//compile-time parameters
/*
const int J = 8;
const int K = 4;
const int M = 10;
const int N = 10;
*/
const int J = 4;
const int K = 2;
const int M = 3;
const int N = 2;

const int LENGTH = pow(2, J);

struct wireDict
{
	int wire = -1;
	bool sigval = false;
};

int main(int argc, char** argv)
{

	int wirelist[(2 * M * N) + K];
	bool redOut[(M * N)]; //my output from each gate
	bool blackIn[(2 * M * N)];

	string truthTable[LENGTH]; //intial inputs iterated through

	bool jUsed[J];
	int jCols[J];
	
//	wireDict jWires[J];

	bool jVal[J];
	bool kVal[K];

	int tempRed, tempBlack;
	int actualUsed = 0;

	ifstream fin;
	fin.open(argv[1]);

	//initialize jCols to -1
	for (int i = 0; i < J; i++)	
	{
		jCols[i] = -1;
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
	}

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

	cout << actualUsed << endl;

	// fill the truth table (initial values)
	for (int i = 0; i < pow(2, actualUsed); i++) 
	{
		bitset<J> tempTable(i);
		tempTable <<= (J - actualUsed);
		truthTable[i] = tempTable.to_string();
		cout << truthTable[i] << endl;
	}

	//ONE BIG ASS FOR LOOP HERE GODDAMMIT!

	//some fucking way I need to get my original j's (truthTable[i]) into my black inputs
	//Do I have this already???????????? NO I FUCKING DO NOT HAVE THIS
	for (int i = 0; i < actualUsed; i++)//Do I fucking j loop or length loop.
	//pretty sure it's 2 * M * N.  
	{
			jVal[jCols[i]] = (truthTable[3][i] == '1');
	}
	for (int i = 0; i < J; i++)
	{
		cout << jVal[i] << " "; //output makes sense
	}

	//ok need to figure out how to know which j to send the signal from truth table to

	//NAND and store into array indexed by red (redOut)
	for (int i = 0; i < (2 * M * N) + K; i +=2)
	{
		//if (wirelist[i] != -1 && wirelist[i+1] != -1)
			//redOut[i/2] = nand();
	}

	//alright, Ben.  Goal:



	//close wiring list
	fin.close();
	return 0;
}

bool nand(bool in1, bool in2)
{
	return !(in1 && in2);
}

//Probably uncessary anymore
/*
int mapBlackToRed(int blackNum)
{
	if (blackNum % 2 ==1)
	{
		blackNum--;
		blackNum = blackNum / 2;
		return blackNum + J;
	}
	else
	{
		blackNum = blackNum / 2;
		return blackNum + J;
	}
}
*/
/*
int* mapRedToBlack(int redNum)
{
	static int blacks[2];
	blacks[0] = (redNum - J) * 2;
	blacks[1] = ((redNum - J) * 2) + 1;
	return blacks;
}
*/
/*
int getCol(int blackNum)
{
	if (blackNum < (2 * ((M * N))))
	{
		return blackNum/(2*M);
	}
	return -1;
}
*/
/*
int getRow(int redNum)
{
	if (redNum - J >= 0)
	{
		return (redNum - J) % M;
	}
	return -1;
}
*/
/*
int* map2Darray(int redNum)
{
	static int coords[2];
	coords[0] = getRow(redNum);
	int* temp = mapRedToBlack(redNum);
	coords[1] = getCol(temp[1]);
	return coords;
}
*/
