
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
			if (aiBoard & moveMask) {
				field[5 - j][i] = "x";
			}
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
		std::cout << "ai has won backslash" << std::endl;
		return true;
	}
	tempBoard = playerBoard & (playerBoard >> 7);
	if (tempBoard & (tempBoard >> 2 * 7)) { // check horizontal -
		std::cout << "ai has won horizontal" << std::endl;
		return true;
	}
	tempBoard = playerBoard & (playerBoard >> 8);
	if (tempBoard & (tempBoard >> 2 * 8)) { // check / diagonal
		std::cout << "ai has won slash" << std::endl;
		return true;
	}
	tempBoard = playerBoard & (playerBoard >> 1);
	if (tempBoard & (tempBoard >> 2)) {   // check vertical |
		std::cout << "ai has won vertical" << std::endl;
		return true;
	}
	return false;
	return false;
}

bool BitboardField::aiWonCheck()
{
	unsigned __int64 tempBoard = aiBoard & (aiBoard >> 6);
	if (tempBoard & (tempBoard >> 2 * 6)) { // check \ diagonal
		std::cout << "ai has won backslash" << std::endl;
		return true;
	}
	tempBoard = aiBoard & (aiBoard >> 7);
	if (tempBoard & (tempBoard >> 2 * 7)) { // check horizontal -
		std::cout << "ai has won horizontal" << std::endl;
		return true;
	}
	tempBoard = aiBoard & (aiBoard >> 8);
	if (tempBoard & (tempBoard >> 2 * 8)) { // check / diagonal
		std::cout << "ai has won slash" << std::endl;
		return true;
	}
	tempBoard = aiBoard & (aiBoard >> 1);
	if (tempBoard & (tempBoard >> 2)) {   // check vertical |
		std::cout << "ai has won vertical" << std::endl;
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
			aiBoard = aiBoard & moveMask;
		}
		moveMask = moveMask << 1;
	}
}

void BitboardField::playerAddMove(int a)
{
	unsigned __int64 combBoard = aiBoard | playerBoard;
	unsigned __int64 moveMask = pow(2, a * 7);
	for (int i = 0; i < 6; i++)
	{
		if (!(combBoard & moveMask)) {
			playerBoard = playerBoard & moveMask;
		}
		moveMask = moveMask << 1;
	}
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
	return 0;
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
	unsigned __int64 check;
	for (int i = 0; i < 6; i++) {
		if (check & playerBoard) {
			field[i][pastMove] = " ";
			break;
		}
		if (check & aiBoard) {
			
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
	return 0;
}

