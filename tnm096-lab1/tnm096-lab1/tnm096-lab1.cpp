// tnm096-lab1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <utility>  
#include <vector>
#include <algorithm>
const int EmptySlot = 0;

const int startState[3][3] = { { 1,EmptySlot,2 }, { 4,5,3 }, {7,8,6} }; 
// unsolvable const int startState[3][3] = { { 8,1,2}, { EmptySlot,4,3 }, {7,6,5} };
const int endState[3][3] = { { 1,2,3 }, { 4,5,6 }, {7,8,EmptySlot} };

struct Puzzle {
	int currentPuzzle[3][3];
	std::pair<int,int> emptyslot;
	int h1;
	int h2;
};
struct PuzzleNode {
	Puzzle puzzle;
	int parent;

};
// print operator for puzzles
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

bool isSolvable(Puzzle& p)
{
	int inversions = 0;
	int steps = 3;
	int lastVal;
	for (int j = 0; j < steps; j++) {
		for (int i = 0; i < steps; i++)
		{
			if(i == 0 && j==0)
				lastVal = p.currentPuzzle[j][i];
			else
			{
				if (lastVal > p.currentPuzzle[j][i])
					inversions++;
			}
		}
	}
	// If the amount of inversions is even the puzzle can be solved
	return inversions % 2 == 0;

}
// compares states (content of 2 puzzles
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
/* following section is for checking if blank can be moved*/
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

// how wrong is current state (uses h1)
int missplacedNumbers(Puzzle& myP, Puzzle& solved)
{
	int missplaced = 0;
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++)
		{
			if (myP.currentPuzzle[j][i] != solved.currentPuzzle[j][i] && myP.currentPuzzle[j][i] != 0)
			{
				missplaced++;
			}
		}
	}
	return missplaced;
}
// create new puzzle
void setupPuzzle(Puzzle& p , const int state[3][3],Puzzle& endStatePuzzle) {
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
	int nr = missplacedNumbers(p, endStatePuzzle);
	p.h1 = nr;
}
// 
bool puzzleSorth1 ( PuzzleNode & lhs, PuzzleNode& rhs)
{//We need to overload "<" to put our struct into a set
	return lhs.puzzle.h1 < rhs.puzzle.h1;
}
// check if it exist in our vector of puzzles(because equal slides means later added is optimal)
bool puzzleExists(Puzzle& currentPuzzle, std::vector<PuzzleNode>& currentNode)
{
	for (int i = 0; i < currentNode.size(); i++)
		if (isSame(currentNode[i].puzzle, currentPuzzle))
			return true;
	return false;
}
Puzzle createNextPuzzle(Puzzle cP, std::pair<int, int> pos)
{
	int temp = cP.currentPuzzle[pos.first][pos.second];
	cP.currentPuzzle[cP.emptyslot.first][cP.emptyslot.second] = temp;
	cP.currentPuzzle[pos.first][pos.second] = 0;
	cP.emptyslot = pos;
	return cP;
}
void solvePuzzle(std::vector<PuzzleNode>& currentNode, int i, std::pair<int,int> futurePos)
{
	Puzzle tempCP = createNextPuzzle(currentNode[i].puzzle, futurePos);

	if (!puzzleExists(tempCP, currentNode))
	{
		PuzzleNode newNode = { tempCP,i };

		std::cout << tempCP;
		currentNode.push_back(newNode);
	}
}


int main()
{
	Puzzle myPuzzle;
	Puzzle endStatePuzzle;
	setupPuzzle(endStatePuzzle, endState, endStatePuzzle);
	setupPuzzle(myPuzzle, startState, endStatePuzzle);

	std::vector<PuzzleNode> open;
	std::vector<PuzzleNode> closed;

	if (isSolvable(myPuzzle)) {
		std::cout << myPuzzle << std::endl << "is not solvable";
	}
	else {
		int counter = 0;
		PuzzleNode root = { myPuzzle,-1 };
		open.push_back(root);
		//loop that solves it
		while (counter < open.size() && !isSame(open[counter].puzzle, endStatePuzzle))
		{
			Puzzle tempPuzzle = open[counter].puzzle;
			std::pair<int, int> emptyLocation = tempPuzzle.emptyslot;

			//check all possible moves
			if (Bup(tempPuzzle))
				solvePuzzle(open, counter, std::make_pair(emptyLocation.first - 1, emptyLocation.second));
			if (Bright(tempPuzzle))
				solvePuzzle(open, counter, std::make_pair(emptyLocation.first, emptyLocation.second + 1));
			if (Bdown(tempPuzzle))
				solvePuzzle(open, counter, std::make_pair(emptyLocation.first + 1, emptyLocation.second));
			if (Bleft(tempPuzzle))
				solvePuzzle(open, counter, std::make_pair(emptyLocation.first, emptyLocation.second - 1));

			counter++;

		}


		std::cout << "found solution" << std::endl;
		int parentpointer = counter;
		std::vector<Puzzle> printvector;
		int numberOfMoves = 0;
		while (parentpointer != -1)
		{
			printvector.insert(printvector.begin(), open[parentpointer].puzzle);
			parentpointer = open[parentpointer].parent;
			numberOfMoves++;
		}
		for (auto it = printvector.begin(); it != printvector.end(); ++it)
			std::cout << *it << " ";
	}

	
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
