
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

//default constructor
BitboardField::BitboardField()
{
}

//unused constructor may be useful later
BitboardField::BitboardField(std::vector<unsigned __int64> openings)
{
	book = openings;
}

//constructor that sets up evaluation table and opening book
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

//convert bitboard into an array this is used for debug purposes
void BitboardField::convertIntoArray()
{
	unsigned __int64 moveMask = 0b0000000000000000000000000000000000000000000000000000000000000001;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 6; j++) {
			if (playerBoard & moveMask) {
				field[5-j][i] = "o";
			}
			else if (aiBoard & moveMask) {
				field[5 - j][i] = "x";
			}
			else {
				field[5 - j][i] = " ";
			}
			moveMask = moveMask << 1;
		}
		moveMask = moveMask << 1;
	}
}

//print field to console
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

//check if player won
bool BitboardField::playerWonCheck()
{
	unsigned __int64 tempBoard = playerBoard & (playerBoard >> 6);
	if (tempBoard & (tempBoard >> 2 * 6)) { // check \ diagonal
	//	std::cout << "player has won backslash" << std::endl;
		return true;
	}
	tempBoard = playerBoard & (playerBoard >> 7);
	if (tempBoard & (tempBoard >> 2 * 7)) { // check horizontal -
	//	std::cout << "player has won horizontal" << std::endl;
		return true;
	}
	tempBoard = playerBoard & (playerBoard >> 8);
	if (tempBoard & (tempBoard >> 2 * 8)) { // check / diagonal
	//	std::cout << "player has won slash" << std::endl;
		return true;
	}
	tempBoard = playerBoard & (playerBoard >> 1);
	if (tempBoard & (tempBoard >> 2)) {   // check vertical |
	//	std::cout << "player has won vertical" << std::endl;
		return true;
	}
	return false;
	return false;
}

//check if ai won
bool BitboardField::aiWonCheck()
{
	unsigned __int64 tempBoard = aiBoard & (aiBoard >> 6);
	if (tempBoard & (tempBoard >> 2 * 6)) { // check \ diagonal
		//std::cout << "ai has won backslash" << std::endl;
		return true;
	}
	tempBoard = aiBoard & (aiBoard >> 7);
	if (tempBoard & (tempBoard >> 2 * 7)) { // check horizontal -
	//	std::cout << "ai has won horizontal" << std::endl;
		return true;
	}
	tempBoard = aiBoard & (aiBoard >> 8);
	if (tempBoard & (tempBoard >> 2 * 8)) { // check / diagonal
		//std::cout << "ai has won slash" << std::endl;
		return true;
	}
	tempBoard = aiBoard & (aiBoard >> 1);
	if (tempBoard & (tempBoard >> 2)) {   // check vertical |
		//std::cout << "ai has won vertical" << std::endl;
		return true;
	}
	return false;
}

//add ai move
void BitboardField::aiAddMove(int a)
{
	unsigned __int64 combBoard = aiBoard | playerBoard;
	unsigned __int64 moveMask = pow(2, a * 7);
	for (int i = 0; i < 6; i++)
	{
		if (!(combBoard & moveMask)) {
			aiBoard = aiBoard | moveMask;
			break;
		}
		moveMask = moveMask << 1;
	}
	pastMoves.push_back(a);
}

//add player move
void BitboardField::playerAddMove(int a)
{
	unsigned __int64 combBoard = aiBoard | playerBoard;
	unsigned __int64 moveMask = pow(2, a * 7);
	for (int i = 0; i < 6; i++)
	{
		if (!(combBoard & moveMask)) {
			playerBoard = playerBoard | moveMask;
			break;
		}
		moveMask = moveMask << 1;
	}
	pastMoves.push_back(a);
}

//check if move is valid
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

//standard evaluate node function for board that isint a draw loss or win
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
}

//return all possible moves for ai
std::vector<int> BitboardField::genMoves()
{
	unsigned __int64 c0 = 0b0000000000000000000000000000000000000000000000000000000000100000;
	unsigned __int64 c1 = 0b0000000000000000000000000000000000000000000000000001000000000000;
	unsigned __int64 c2 = 0b0000000000000000000000000000000000000000000010000000000000000000;
	unsigned __int64 c3 = 0b0000000000000000000000000000000000000100000000000000000000000000;
	unsigned __int64 c4 = 0b0000000000000000000000000000001000000000000000000000000000000000;
	unsigned __int64 c5 = 0b0000000000000000000000010000000000000000000000000000000000000000;
	unsigned __int64 c6 = 0b0000000000000000100000000000000000000000000000000000000000000000;
	unsigned __int64 comboBoard = aiBoard | playerBoard;
	std::vector<int> v;

	if (!(c3 & comboBoard))
		v.push_back(3);
	if (!(c4 & comboBoard))
		v.push_back(4);
	if (!(c2 & comboBoard))
		v.push_back(2);
	if (!(c5 & comboBoard))
		v.push_back(5);
	if (!(c1 & comboBoard))
		v.push_back(1);
	if (!(c6 & comboBoard))
		v.push_back(6);
	if (!(c0 & comboBoard))
		v.push_back(0);

	/* old genMoves
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
	*/
	return v;
}

//undo last move
void BitboardField::undo()
{
	int pastMove = pastMoves[pastMoves.size() - 1];
	pastMoves.pop_back();
	unsigned __int64 check = pow(2, pastMove * 7 + 5);
	for (int i = 0; i < 6; i++) {
		if (check & playerBoard) {
			playerBoard = check ^ playerBoard;
			break;
		}
		if (check & aiBoard) {
			aiBoard = check ^ aiBoard;
			break;
		}
		check = check >> 1;
	}
}

//check if game is a draw
bool BitboardField::draw()
{
	if (pastMoves.size() == 42)
		return true;
	return false;
}

//return current number of moves played
int BitboardField::getMoveCount()
{
	return pastMoves.size();
}

//only used for opening book checks
void BitboardField::flip() {
	unsigned __int64 leftSide = 0;
	unsigned __int64 rightSide = 0;
	unsigned __int64 middle = 0;
	unsigned __int64 check = 1;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 6; j++) {
			if (check & aiBoard) {
				rightSide += pow(2, 42 - (7*i) + j);
			}
			check = check << 1;
		}
		check = check << 1;
	}

	for (int j = 0; j < 6; j++) {
		if (check & aiBoard) {
			leftSide += pow(2, 21 + j);
		}
		check = check << 1;
	}
	check = check << 1;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 6; j++) {
			if (check & aiBoard) {
				leftSide += pow(2, 14 - (7 * i) + j);
			}
			check = check << 1;
		}
		check = check << 1;
	}
	aiBoard = leftSide | rightSide | middle;

	leftSide = 0;
	rightSide = 0;
	middle = 0;
	check = 1;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 6; j++) {
			if (check & playerBoard) {
				rightSide += pow(2, 42 - (7 * i) + j);
			}
			check = check << 1;
		}
		check = check << 1;
	}

	for (int j = 0; j < 6; j++) {
		if (check & playerBoard) {
			leftSide += pow(2, 21 + j);
		}
		check = check << 1;
	}
	check = check << 1;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 6; j++) {
			if (check & playerBoard) {
				leftSide += pow(2, 14 - (7 * i) + j);
			}
			check = check << 1;
		}
		check = check << 1;
	}
	playerBoard = leftSide | rightSide | middle;
}

//checks if board state is in the opening book
int BitboardField::evaluateBook(bool playfirst)
{
	if (playfirst) {
		for (int k = 0; k < 67557; k++) {
			//possible issue test later
			if ((book[k * 2] == aiBoard) && (book[k * 2 + 1] == playerBoard)) {
				if (bookResults[k] == "win") {
					//	std::cout << "found a win" << std::endl;
					return 1000;
				}

				if (bookResults[k] == "loss") {
					//		std::cout << "found a loss" << std::endl;
					return -1000;
				}
				if (bookResults[k] == "draw") {
					//	std::cout << "found a draw" << std::endl;
					return 0;
				}

			}
		}
		flip();
		for (int k = 0; k < 67557; k++) {
			//possible issue test later
			if ((book[k * 2] == aiBoard) && (book[k * 2 + 1] == playerBoard)) {
				if (bookResults[k] == "win") {
					//	std::cout << "found a win" << std::endl;
					flip();
					return 1000;
				}

				if (bookResults[k] == "loss") {
					flip();
					//		std::cout << "found a loss" << std::endl;
					return -1000;
				}
				if (bookResults[k] == "draw") {
					flip();
					//	std::cout << "found a draw" << std::endl;
					return 0;
				}

			}
		}
		flip();
	}
	else
	{
		for (int k = 0; k < 67557; k++) {
			//possible issue test later
			if ((book[k * 2] == playerBoard) && (book[k * 2 + 1] == aiBoard)) {
				if (bookResults[k] == "win") {
					//	std::cout << "found a win" << std::endl;
					return -1000;
				}

				if (bookResults[k] == "loss") {
					//		std::cout << "found a loss" << std::endl;
					return 1000;
				}
				if (bookResults[k] == "draw") {
					//	std::cout << "found a draw" << std::endl;
					return 0;
				}

			}
		}
		flip();
		for (int k = 0; k < 67557; k++) {
			//possible issue test later
			if ((book[k * 2] == playerBoard) && (book[k * 2 + 1] == aiBoard)) {
				if (bookResults[k] == "win") {
					//	std::cout << "found a win" << std::endl;
					flip();
					return -1000;
				}

				if (bookResults[k] == "loss") {
					flip();
					//		std::cout << "found a loss" << std::endl;
					return 1000;
				}
				if (bookResults[k] == "draw") {
					flip();
					//	std::cout << "found a draw" << std::endl;
					return 0;
				}

			}
		}
		flip();
	}
//	printf("not in book ????????");
	return -10000;
}

