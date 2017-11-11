
#include "stdafx.h"
#include "BitboardField.h"
#include <fstream>
#include <bitset>

BitboardField::BitboardField(std::string fileName)
{
	aiBoard = 0b1010000000010101000000000011110000000000101001001001001010101010;
	playerBoard = 0b1010000000010101000000000011110000000000101001001001001010101010;
	std::cout << std::bitset<64>(aiBoard) << std::endl;
	playerBoard = playerBoard >> 1;
	aiBoard = aiBoard & playerBoard;
	playerBoard = playerBoard << 2 * 5;
	std::cout << std::bitset<64>(aiBoard);

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
			for (int i = 0; i < 83; i += 2) {
				if (line[i] == 'b')
					;
					//book[count][5 - (i / 2) % 6][(i / 2) / 6] = " ";
				else
					;
					//book[count][5 - (i / 2) % 6][(i / 2) / 6] += line[i];
			}
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

void BitboardField::printField()
{
}

bool BitboardField::playerWonCheck()
{
	return false;
}

bool BitboardField::aiWonCheck()
{
	unsigned __int64 y = aiBoard & (aiBoard >> 6);
	if (y & (y >> 2 * 6)) // check \ diagonal
		return true;
	y = aiBoard & (aiBoard >> 7);
	if (y & (y >> 2 * 7)) // check horizontal -
		return true;
	y = aiBoard & (aiBoard >> 8);
	if (y & (y >> 2 * 8)) // check / diagonal
		return true;
	y = aiBoard & (aiBoard >> 1);
	if (y & (y >> 2))     // check vertical |
		return true;
	return false;
}

void BitboardField::aiAddMove(int a)
{
}

void BitboardField::playerAddMove(int a)
{
}

bool BitboardField::validMove(std::string move)
{
	return false;
}

int BitboardField::evaluate()
{
	return 0;
}

std::vector<int> BitboardField::genMoves()
{
	return std::vector<int>();
}

void BitboardField::undo()
{
}

bool BitboardField::draw()
{
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
