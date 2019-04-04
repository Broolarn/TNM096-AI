// tnm096-lab1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <utility>  
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

const int EmptySlot = 0;

//const int startState[3][3] = { { 1,EmptySlot,2 }, { 4,5,3 }, {7,8,6} }; // 3 moves
const int startState[3][3] = { { 1,3,2 }, { 4,6,5 }, {7,EmptySlot,8} };
//const int startState[3][3] = { { 6,4,7 }, { 8,5,EmptySlot }, {3,2,1} };
// unsolvable const int startState[3][3] = { { 8,1,2}, { EmptySlot,4,3 }, {7,6,5} };
const int endState[3][3] = { { 1,2,3 }, { 4,5,6 }, {7,8,EmptySlot} };

struct Puzzle {
	int currentPuzzle[3][3];
	std::pair<int, int> emptyslot;
	int h1;
	int h2;
};
struct PuzzleNode {
	Puzzle puzzle;
	PuzzleNode* parent;

};
// print operator for puzzles
std::ostream& operator<<(std::ostream& stream, Puzzle& p)
{
	int steps = 3;
	stream << std::endl << "============================" << std::endl;
	for (int j = 0; j < steps; j++) {
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
			if (i == 0 && j == 0)
				lastVal = p.currentPuzzle[j][i];
			else
			{
				
				if (lastVal > p.currentPuzzle[j][i])
					inversions++;
				lastVal = p.currentPuzzle[j][i];
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

int manhattan(Puzzle& myP, Puzzle& solved)
{
	int misplaced = 0;
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++)
		{
			if (myP.currentPuzzle[j][i] != solved.currentPuzzle[j][i] && myP.currentPuzzle[j][i] != 0)
			{
				for (int m = 0; m < 3; m++)
					for(int n = 0; n < 3; n++)
					{
						if (myP.currentPuzzle[j][i] == solved.currentPuzzle[m][n])
							misplaced += abs(j - m) + abs(i - n);
					}
			}
		}
	}
	return misplaced; 
}
// create new puzzle
void setupPuzzle(Puzzle& p, const int state[3][3], Puzzle& endStatePuzzle) {
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
	p.h2 = manhattan(p, endStatePuzzle);
}
// 
bool puzzleSorth1(PuzzleNode & lhs, PuzzleNode& rhs)
{//We need to overload "<" to put our struct into a set
	return lhs.puzzle.h1 > rhs.puzzle.h1;
}
bool puzzleSorth2(PuzzleNode & lhs, PuzzleNode& rhs)
{//We need to overload "<" to put our struct into a set
	return lhs.puzzle.h2 > rhs.puzzle.h2;
}
// check if it exist in our vector of puzzles(because equal slides means later added is optimal)
bool puzzleExists(Puzzle& currentPuzzle, std::vector<PuzzleNode>& currentNode)
{
	for (int i = 0; i < currentNode.size(); i++)
		if (isSame(currentNode[i].puzzle, currentPuzzle))
			return true;
	return false;
}
Puzzle createNextPuzzle(Puzzle cP, std::pair<int, int> pos, Puzzle& goal, int i)
{
	int temp = cP.currentPuzzle[pos.first][pos.second];
	cP.currentPuzzle[cP.emptyslot.first][cP.emptyslot.second] = temp;
	cP.currentPuzzle[pos.first][pos.second] = 0;
	cP.emptyslot = pos;
	cP.h1 = i + missplacedNumbers(cP, goal);
	cP.h2 = i + manhattan(cP, goal);
	return cP;
}
int traversetree(PuzzleNode pN) {

	PuzzleNode* lastpoint = nullptr;
	PuzzleNode* temp = &pN;
	int counter = 0;
	while (temp != nullptr)
	{
		counter++;
		lastpoint = temp;
		temp = temp->parent;
		
	}
	return counter;


}

bool solvePuzzle(std::vector<PuzzleNode>& open, std::vector<PuzzleNode> closed, PuzzleNode tempP, int i, std::pair<int, int> futurePos, Puzzle& goal, PuzzleNode& answer, bool sorting)
{
	Puzzle tempCP = createNextPuzzle(tempP.puzzle, futurePos, goal, traversetree(tempP));

	if (isSame(tempCP, goal))
	{
		PuzzleNode* tempCPPtr = new PuzzleNode(closed.back());
		PuzzleNode newNode = { tempCP,tempCPPtr };
		open.push_back(newNode);
		if (sorting == true)
			std::push_heap(open.begin(), open.end(), puzzleSorth1);
		else
			std::push_heap(open.begin(), open.end(), puzzleSorth2);

		answer = newNode;
		//std::cout << tempCP;
		return true;
	}

	else if (!puzzleExists(tempCP, closed) && !puzzleExists(tempCP, open))
	{
		PuzzleNode* tempCPPtr = new PuzzleNode(closed.back());
		PuzzleNode newNode = { tempCP,tempCPPtr };

	//	std::cout << tempCP;

		open.push_back(newNode);
		if (sorting == true)
			std::push_heap(open.begin(), open.end(), puzzleSorth1);
		else
			std::push_heap(open.begin(), open.end(), puzzleSorth2);
	}

	return false;
}

// random generator function:
int myrandom(int i) { return std::rand() % i; }

int main()
{
	Puzzle myPuzzle;
	Puzzle endStatePuzzle;
	setupPuzzle(endStatePuzzle, endState, endStatePuzzle);
	setupPuzzle(myPuzzle, startState, endStatePuzzle);// startState 

	std::vector<PuzzleNode> open;
	std::vector<PuzzleNode> closed;
	bool sortingh1 = false;
	std::string path = "";

	if (isSolvable(myPuzzle)) {
		std::cout << myPuzzle << std::endl << "is not solvable";
	}
	else {
		int counter = 0;
		PuzzleNode root = { myPuzzle,nullptr};
		if (sortingh1 == true)
		{
			std::make_heap(open.begin(), open.end(), puzzleSorth1);
			open.push_back(root);
			std::push_heap(open.begin(), open.end(), puzzleSorth1);
		}

		else
		{
			std::make_heap(open.begin(), open.end(), puzzleSorth2);
			open.push_back(root);
			std::push_heap(open.begin(), open.end(), puzzleSorth2);
		}
		
		PuzzleNode answer;
		//loop that solves it
		while (!open.empty() && !isSame(open.front().puzzle, endStatePuzzle) )
		{
			PuzzleNode tempNode = open.front();

			Puzzle tempPuzzle = open.front().puzzle;
			closed.push_back(open.front());
			if (sortingh1 == true)
				std::pop_heap(open.begin(), open.end(), puzzleSorth1);
			else
				std::pop_heap(open.begin(), open.end(), puzzleSorth2);

			open.pop_back();
			std::pair<int, int> emptyLocation = tempPuzzle.emptyslot;

			bool completed = false;
			//check all possible moves
			if (Bup(tempPuzzle))
			{
				completed = solvePuzzle(open, closed, tempNode, counter, std::make_pair(emptyLocation.first - 1, emptyLocation.second), endStatePuzzle, answer, sortingh1);
				if (completed == true)
					break;
			}

			if (Bright(tempPuzzle))
			{
				completed = solvePuzzle(open, closed, tempNode, counter, std::make_pair(emptyLocation.first, emptyLocation.second + 1), endStatePuzzle, answer, sortingh1);
				if (completed == true)
					break;
			}

			if (Bdown(tempPuzzle))
			{
				completed = solvePuzzle(open, closed, tempNode, counter, std::make_pair(emptyLocation.first + 1, emptyLocation.second), endStatePuzzle, answer, sortingh1);
				if (completed == true)
					break;
			}

			if (Bleft(tempPuzzle))
			{
				completed = solvePuzzle(open, closed, tempNode, counter, std::make_pair(emptyLocation.first, emptyLocation.second - 1), endStatePuzzle, answer, sortingh1);
				if (completed == true)
					break;
			}				
			counter++;

		}


		//std::sort_heap(open.begin(), open.end(), puzzleSorth2);
		
		std::vector<Puzzle> printvector;
		std::vector<std::pair<int,int>> dirrvec;
		int numberOfMoves = 0;
		std::cout << "found solution" << std::endl;
		
		PuzzleNode traverseNode = answer;
		PuzzleNode* parentpointer = traverseNode.parent;
		while (parentpointer != nullptr)
		{
			numberOfMoves++;
			dirrvec.insert(dirrvec.begin(), traverseNode.puzzle.emptyslot);
			printvector.insert(printvector.begin(),traverseNode.puzzle);
			traverseNode = *parentpointer;
			parentpointer = parentpointer->parent;
			
		}
		for (auto it = printvector.begin(); it != printvector.end(); ++it)
			std::cout << *it << " ";

		bool first = true;
		auto last = dirrvec.begin();
		for (auto it = dirrvec.begin(); it != dirrvec.end(); ++it) {
			if (first)
			{
				last = it;
				first = false;
			}
				
			if (last != it)
			{
				if (last->first < it->first)
					std::cout << "Down ";
				else if (last->first > it->first)
					std::cout << "Up ";
				else if (last->second < it->second)
					std::cout << "Right ";
				else
					std::cout << "Left ";
				
			}
			last = it;
		}
			

		std::cout << "Number of moves: " << numberOfMoves;
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
