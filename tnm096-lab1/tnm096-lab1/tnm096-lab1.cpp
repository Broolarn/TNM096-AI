// tnm096-lab1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <utility>  
#include <list>

struct Puzzle {
	int currentPuzzle[3][3];
	std::pair<int,int> emptyslot;
};


std::ostream& operator<<(std::ostream& stream,Puzzle& p)
{
	int steps = 3;
	stream << std::endl <<  "============================" << std::endl;
	for (int j = 0; j < steps; j++){
		for (int i = 0; i < steps; i++)
		{
			stream << p.currentPuzzle[j][i] << " ";
		}
		std::cout << std::endl;
	}
	stream << "============================" << std::endl;
	return stream;
}

bool isSame(Puzzle& state1, Puzzle& state2)
{
	int steps = 3;
	
	for (int j = 0; j < steps; j++) {
		for (int i = 0; i < steps; i++)
		{
			if (state1.currentPuzzle[j][i] != state2.currentPuzzle[j][i]) {
				return false;
			}
		}
	}
	return true;
}

bool Bup(Puzzle & p)
{
	return p.emptyslot.first > 0;
}
bool Bdown(Puzzle & p)
{
	return p.emptyslot.first < 2;
}

bool Bleft(Puzzle & p)
{
	return p.emptyslot.second > 0;
}

bool Bright(Puzzle & p)
{
	return p.emptyslot.second < 2;
}





void setupPuzzle(Puzzle& p , int state[3][3]) {
	int EmptySlot = 0;
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++)
		{
			p.currentPuzzle[j][i] = state[j][i];
			
			if (state[j][i] == EmptySlot)
			{
				p.emptyslot = std::make_pair(j, i);
			}
		}
	}
}
int main()
{
	int EmptySlot = 0;
	Puzzle myPuzzle;
	Puzzle endStatePuzzle;
	int endState[3][3] = { { 1,2,3 }, { 4,5,6 }, {7,8,EmptySlot} };
	int startState[3][3] = { { 1,2,3 }, { 4,5,6 }, {7,8,EmptySlot} };
	setupPuzzle(myPuzzle, startState);
	setupPuzzle(endStatePuzzle, endState);
	

	std::cout << myPuzzle;
	if (isSame(myPuzzle, endStatePuzzle))
	{
		std::cout << "same" << std::endl;
	}
	else
		std::cout << "not same" << std::endl;

	std::list<int> open;
	std::list<int> closed;


	if (Bup(myPuzzle))
	{
		std::cout << "up";
	};
	if (Bdown(myPuzzle))
	{
		std::cout << "down";
	};
	if (Bleft(myPuzzle))
	{
		std::cout << "left";
	};
	if (Bright(myPuzzle))
	{
		std::cout << "right";
	};
    std::cout << "Hello World!\n"; 
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
