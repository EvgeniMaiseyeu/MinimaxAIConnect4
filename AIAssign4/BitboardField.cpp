
#include "stdafx.h"
#include "BitboardField.h"
#include <fstream>
#include <math.h>
#include <bitset>

//5  12  19  26  33  40  47
//4  11  18  25  32  39  46
//3  10  17  24  31  38  45
//2  9   16  23  30  37  44
//1  8   15  22  29  36  43
//0  7   14  21  28  35  42

BitboardField::BitboardField()
{
}

BitboardField::BitboardField(std::vector<unsigned __int64> openings)
{
	book = openings;
}

BitboardField::BitboardField(std::string fileName)
{
	//std::cout << std::bitset<64>(aiBoard);

	evaluationTable[0][0] = 3;
	evaluationTable[0][1] = 4;
	evaluationTable[0][2] = 5;
	evaluationTable[0][3] = 7;
	evaluationTable[0][4] = 5;
	evaluationTable[0][5] = 4;
	evaluationTable[0][6] = 3;

	evaluationTable[1][0] = 4;
	evaluationTable[1][1] = 6;
	evaluationTable[1][2] = 8;
	evaluationTable[1][3] = 10;
	evaluationTable[1][4] = 8;
	evaluationTable[1][5] = 6;
	evaluationTable[1][6] = 4;

	evaluationTable[2][0] = 5;
	evaluationTable[2][1] = 8;
	evaluationTable[2][2] = 11;
	evaluationTable[2][3] = 13;
	evaluationTable[2][4] = 11;
	evaluationTable[2][5] = 8;
	evaluationTable[2][6] = 5;

	evaluationTable[3][0] = 5;
	evaluationTable[3][1] = 8;
	evaluationTable[3][2] = 11;
	evaluationTable[3][3] = 13;
	evaluationTable[3][4] = 11;
	evaluationTable[3][5] = 8;
	evaluationTable[3][6] = 5;

	evaluationTable[4][0] = 4;
	evaluationTable[4][1] = 6;
	evaluationTable[4][2] = 8;
	evaluationTable[4][3] = 10;
	evaluationTable[4][4] = 8;
	evaluationTable[4][5] = 6;
	evaluationTable[4][6] = 4;

	evaluationTable[5][0] = 3;
	evaluationTable[5][1] = 4;
	evaluationTable[5][2] = 5;
	evaluationTable[5][3] = 7;
	evaluationTable[5][4] = 5;
	evaluationTable[5][5] = 4;
	evaluationTable[5][6] = 3;

	std::string line;
	std::ifstream myfile(fileName);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{	
			unsigned __int64 aiTempBoard = blankBoard;
			unsigned __int64 playerTempBoard = blankBoard;
			for (int i = 0; i < 83; i += 2) {
				if (line[i] == 'x') {
					aiTempBoard += pow(2, i/2 + floor(i/2 / 6));
				}
				else if (line[i] == 'o') {
					playerTempBoard += pow(2, i / 2 + floor(i/2 / 6));
				}
			}
			book.push_back(aiTempBoard);
			book.push_back(playerTempBoard);
			if (line.length() == 87)
				bookResults.push_back("win");
			else if (line[86] == 's')
				bookResults.push_back("loss");
			else
				bookResults.push_back("draw");
		}
		myfile.close();
	}
}

void BitboardField::convertIntoArray()
{
	unsigned __int64 moveMask = 0b0000000000000000000000000000000000000000000000000000000000000001;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 6; j++) {
			if (playerBoard & moveMask) {
				field[5-j][i] = "o";
			}
<<<<<<< HEAD
			else if (aiBoard & moveMask) {
				field[5 - j][i] = "x";
			}
			else {
				field[5 - j][i] = " ";
			}
=======
			if (aiBoard & moveMask) {
				field[5 - j][i] = "x";
			}
>>>>>>> f14ddf31a04aef9441ba95b23f4b4b7faa27974e
			moveMask = moveMask << 1;
		}
		moveMask = moveMask << 1;
	}
}

void BitboardField::printField()
{
	convertIntoArray();
	for (int i = 0; i < 6; i++) {
		std::cout << "\t-----------------------------" << std::endl;
		std::cout << "\t| " + field[i][0] + " | " + field[i][1] + " | " + field[i][2] + " | " + field[i][3]
			+ " | " + field[i][4] + " | " + field[i][5] + " | " + field[i][6] + " |" << std::endl;
	}
	std::cout << "\t-----------------------------" << std::endl;
	std::cout << "\t  0   1   2   3   4   5   6  " << std::endl;
}

bool BitboardField::playerWonCheck()
{
	unsigned __int64 tempBoard = playerBoard & (playerBoard >> 6);
	if (tempBoard & (tempBoard >> 2 * 6)) { // check \ diagonal
<<<<<<< HEAD
	//	std::cout << "player has won backslash" << std::endl;
=======
		std::cout << "ai has won backslash" << std::endl;
>>>>>>> f14ddf31a04aef9441ba95b23f4b4b7faa27974e
		return true;
	}
	tempBoard = playerBoard & (playerBoard >> 7);
	if (tempBoard & (tempBoard >> 2 * 7)) { // check horizontal -
<<<<<<< HEAD
	//	std::cout << "player has won horizontal" << std::endl;
=======
		std::cout << "ai has won horizontal" << std::endl;
>>>>>>> f14ddf31a04aef9441ba95b23f4b4b7faa27974e
		return true;
	}
	tempBoard = playerBoard & (playerBoard >> 8);
	if (tempBoard & (tempBoard >> 2 * 8)) { // check / diagonal
<<<<<<< HEAD
	//	std::cout << "player has won slash" << std::endl;
=======
		std::cout << "ai has won slash" << std::endl;
>>>>>>> f14ddf31a04aef9441ba95b23f4b4b7faa27974e
		return true;
	}
	tempBoard = playerBoard & (playerBoard >> 1);
	if (tempBoard & (tempBoard >> 2)) {   // check vertical |
<<<<<<< HEAD
	//	std::cout << "player has won vertical" << std::endl;
=======
		std::cout << "ai has won vertical" << std::endl;
>>>>>>> f14ddf31a04aef9441ba95b23f4b4b7faa27974e
		return true;
	}
	return false;
	return false;
}

bool BitboardField::aiWonCheck()
{
	unsigned __int64 tempBoard = aiBoard & (aiBoard >> 6);
	if (tempBoard & (tempBoard >> 2 * 6)) { // check \ diagonal
<<<<<<< HEAD
		//std::cout << "ai has won backslash" << std::endl;
=======
		std::cout << "ai has won backslash" << std::endl;
>>>>>>> f14ddf31a04aef9441ba95b23f4b4b7faa27974e
		return true;
	}
	tempBoard = aiBoard & (aiBoard >> 7);
	if (tempBoard & (tempBoard >> 2 * 7)) { // check horizontal -
<<<<<<< HEAD
	//	std::cout << "ai has won horizontal" << std::endl;
=======
		std::cout << "ai has won horizontal" << std::endl;
>>>>>>> f14ddf31a04aef9441ba95b23f4b4b7faa27974e
		return true;
	}
	tempBoard = aiBoard & (aiBoard >> 8);
	if (tempBoard & (tempBoard >> 2 * 8)) { // check / diagonal
<<<<<<< HEAD
		//std::cout << "ai has won slash" << std::endl;
=======
		std::cout << "ai has won slash" << std::endl;
>>>>>>> f14ddf31a04aef9441ba95b23f4b4b7faa27974e
		return true;
	}
	tempBoard = aiBoard & (aiBoard >> 1);
	if (tempBoard & (tempBoard >> 2)) {   // check vertical |
<<<<<<< HEAD
		//std::cout << "ai has won vertical" << std::endl;
=======
		std::cout << "ai has won vertical" << std::endl;
>>>>>>> f14ddf31a04aef9441ba95b23f4b4b7faa27974e
		return true;
	}
	return false;
}

void BitboardField::aiAddMove(int a)
{
	unsigned __int64 combBoard = aiBoard | playerBoard;
	unsigned __int64 moveMask = pow(2, a * 7);
	for (int i = 0; i < 6; i++)
	{
		if (!(combBoard & moveMask)) {
<<<<<<< HEAD
			aiBoard = aiBoard | moveMask;
			break;
		}
		moveMask = moveMask << 1;
	}
	pastMoves.push_back(a);
=======
			aiBoard = aiBoard & moveMask;
		}
		moveMask = moveMask << 1;
	}
>>>>>>> f14ddf31a04aef9441ba95b23f4b4b7faa27974e
}

void BitboardField::playerAddMove(int a)
{
	unsigned __int64 combBoard = aiBoard | playerBoard;
	unsigned __int64 moveMask = pow(2, a * 7);
	for (int i = 0; i < 6; i++)
	{
		if (!(combBoard & moveMask)) {
<<<<<<< HEAD
			playerBoard = playerBoard | moveMask;
			break;
		}
		moveMask = moveMask << 1;
	}
	pastMoves.push_back(a);
=======
			playerBoard = playerBoard & moveMask;
		}
		moveMask = moveMask << 1;
	}
>>>>>>> f14ddf31a04aef9441ba95b23f4b4b7faa27974e
}

bool BitboardField::validMove(std::string move)
{
	unsigned __int64 check;
	if (isdigit(move[0]) && (move[0] - '0') < 7 && move.size() == 1) {
		check = pow(2, (move[0] - '0') * 7 + 5);
		if (!(check & (playerBoard | aiBoard))) {
			return true;
		}
	}
	return false;
}

int BitboardField::evaluate()
{
	int utility = 138;
	int sum = 0;

	unsigned __int64 checker = blankBoard + 1;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 6; j++) {
			if (aiBoard & checker) {
				sum += evaluationTable[5 - j][i];
			}
			else if (playerBoard & checker) {
				sum -= evaluationTable[5 - j][i];
			}
			checker = checker << 1;
		}
		checker = checker << 1;
	}

	return utility + sum;
	/*
	for (int i = 0; i < 6; i++)
		for (int j = 0; j <7; j++)	
			if (field[i][j] == "x")
				sum += evaluationTable[i][j];
			else if (field[i][j] == "o")
				sum -= evaluationTable[i][j];

	return utility + sum; */
}

std::vector<int> BitboardField::genMoves()
{
	unsigned __int64 leftcheck = 0b0000000000000000000000000000000000000100000000000000000000000000;
	unsigned __int64 rightcheck = leftcheck;
	unsigned __int64 comboBoard = aiBoard | playerBoard;
	std::vector<int> v;
	for (int j = 0; j < 4; j++) {
		if (j == 0) {
			if (!(leftcheck & comboBoard))
				v.push_back(3);
		}
		else {
			leftcheck = leftcheck >> 7;
			rightcheck = rightcheck << 7;
			if (!(rightcheck & comboBoard))
				v.push_back(3 + j);
			if (!(leftcheck & comboBoard))
				v.push_back(3 - j);
		}
	}
	return v;
}

void BitboardField::undo()
{
	int pastMove = pastMoves[pastMoves.size() - 1];
	pastMoves.pop_back();
<<<<<<< HEAD
	unsigned __int64 check = pow(2, pastMove * 7 + 5);
	for (int i = 0; i < 6; i++) {
		if (check & playerBoard) {
			playerBoard = check ^ playerBoard;
			break;
		}
		if (check & aiBoard) {
			aiBoard = check ^ aiBoard;
=======
	unsigned __int64 check;
	for (int i = 0; i < 6; i++) {
		if (check & playerBoard) {
			field[i][pastMove] = " ";
			break;
		}
		if (check & aiBoard) {
			
>>>>>>> f14ddf31a04aef9441ba95b23f4b4b7faa27974e
			break;
		}
		check = check >> 1;
	}
}

bool BitboardField::draw()
{
	if (pastMoves.size() == 42)
		return true;
	return false;
}

int BitboardField::getMoveCount()
{
	return pastMoves.size();
}

int BitboardField::evaluateBook(bool playfirst)
{
	bool next = false;
	for (int k = 0; k < 67557; k++) {
		//possible issue test later
		if ((book[k*2] == aiBoard) && (book[k*2+1] == playerBoard)) {
			if (bookResults[k] == "win") {
				//	std::cout << "found a win" << std::endl;
				if (playfirst) return 1000;
				else return -1000;
			}

			if (bookResults[k] == "loss") {
				//		std::cout << "found a loss" << std::endl;
				if (playfirst) return -1000;
				else return 1000;
			}
			if (bookResults[k] == "draw") {
				//	std::cout << "found a draw" << std::endl;
				return 0;
			}
			
		}
	}
	return -10000;
}

