#pragma once
#include "stdafx.h"
#include "GameField.h"
//NOTE
//This is the old version of game field which I do not use anymore, I changed from using 2d array of strings to using bitboards

//game field constructor
GameField::GameField() {
	//initialize field to just contain one space per square
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 7; j++)
			field[i][j] = " ";

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

}

//this function prints the current board position to console when called
void GameField::printField() {
	for (int i = 0; i < 6; i++) {
		std::cout << "\t-----------------------------" << std::endl;
		std::cout << "\t| " + field[i][0] + " | " + field[i][1] + " | " + field[i][2] + " | " + field[i][3] 
			+ " | " + field[i][4] + " | " + field[i][5] + " | " + field[i][6] + " |" << std::endl;
	}
	std::cout << "\t-----------------------------" << std::endl;
	std::cout << "\t  0   1   2   3   4   5   6  " << std::endl;
}

//check if game is a draw
bool GameField::draw() {
	if (pastMoves.size() == 42) 
		return true;
	return false;
}

//add move to game field
void GameField::addMove(int move, std::string p) {
	for (int i = 5; i > -1; i--) {
		if (field[i][move] == " ") {
			field[i][move] = p;
			break;
		}
	}
	pastMoves.push_back(move);
}

//check if passed in move is valid
bool GameField::validMove(std::string move) {
	if (move.size() == 1 && isdigit(move[0]) && (move[0] - '0') < 7 && field[0][(move[0] - '0')] == " ")
		return true;
	return false;
}

//check if someone won
bool GameField::gameOverCheck(std::string pattern) {
	//check vertical conditions
	if (this->pastMoves.size() < 7)
		return false;
	bool traverse = true;
	int i = 5, j = 0;
	int inARow = 0;
	while (traverse) {
		if (i < 6 && j < 7 && i > -1 && j > -1) {
			if (field[i][j] == " ") {
				j++;
				i = 5;
				inARow = 0;
			}
			else if (field[i][j] == pattern) {
				inARow++;
				i--;
			}
			else {
				inARow = 0;
				if (i <= 3) {
					j++;
					i = 5;
					inARow = 0;
				}
				else {
					i--;
				}
			}
			if (inARow == 4)
				return true;
		}
		else
			traverse = false;
	}

	//check horizontal conditions
	traverse = true;
	i = 5;
	j = 0;
	while (traverse) {
		if (i < 6 && j < 7 && i > -1 && j > -1) {
			if (field[i][j] == " ") {
				if (j >= 3) {
					j = 0;
					i--;
				}
				else
					j++;
				inARow = 0;
			}
			else if (field[i][j] == pattern) {
				inARow++;
				j++;
			}
			else {
				inARow = 0;
				if (j >= 3) {
					i--;
					j = 0;
					inARow = 0;
				}
				else {
					j++;
				}
			}
			if (inARow == 4)
				return true;
		}
		else
			traverse = false;
	}

	//check diagonal conditions

	traverse = true;
	i = 2;
	j = 3;
	while (traverse) {
		if (i < 6 && j < 7 && i > -1 && j > -1) {
			if (field[i][j] == " ") {
				i = 2;
				j--;
			}
			else if (field[i][j] == pattern) {
				if (checkRightDownDiag(j, i, pattern))
					return true;
				if (i != 0)
					i--;
				else {
					i = 2;
					j--;
				}
			}
			else {
				if (i == 0) {
					i = 2;
					j--;
				}
				else {
					i--;
				}
			}
		}
		else {
			traverse = false;
		}
	}

	traverse = true;
	i = 2;
	j = 3;
	while (traverse) {
		if (i < 6 && j < 7 && i > -1 && j > -1) {
			if (field[i][j] == " ") {
				i = 2;
				j++;
			}
			else if (field[i][j] == pattern) {
				if (checkLeftDownDiag(j, i, pattern))
					return true;
				if (i != 0)
					i--;
				else {
					i = 2;
					j++;
				}
			}
			else {
				if (i == 0) {
					i = 2;
					j++;
				}
				else {
					i--;
				}
			}
		}
		else {
			traverse = false;
		}
	}
	return false;
}

//used in game over check for left diagonals '/'
bool GameField::checkLeftDownDiag(int x, int y, std::string pat) {
	int i = y;
	int j = x;
	int count = 0;
	while (count < 4) {
		if (i < 6 && j < 7 && i > -1 && j > -1) {
			if (field[i][j] == pat) {
				count++;
				i++;
				j--;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
	return true;
}

//used in game over check for right diagonals '\'
bool GameField::checkRightDownDiag(int x, int y, std::string pat) {
	int i = y;
	int j = x;
	int count = 0;
	while (count < 4) {
		if (i < 6 && j < 7 && i > -1 && j > -1) {
			if (field[i][j] == pat) {
				count++;
				i++;
				j++;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
	return true;
}

//evaluate function to score boards
int GameField::evaluate() {
	
	int utility = 138;
	int sum = 0;
	
	for (int i = 0; i < 6; i++)
		for (int j = 0; j <7; j++)
			if (field[i][j] == "x")
				sum += evaluationTable[i][j];
			else if (field[i][j] == "o")
				sum -= evaluationTable[i][j];

	//add if u block a connected set + something
	//if u increase ur connected set + something
	//
	
	return utility + sum;
}

//generate all possible moves
std::vector<int> GameField::genMoves() {
	std::vector<int> v;
	for (int j = 0; j < 4; j++) {
		if (j == 0) {
			if (field[0][3] == " ")
				v.push_back(3);
		}
		else {
			if (field[0][3 + j] == " ")
				v.push_back(3 + j);
			if (field[0][3 - j] == " ")
				v.push_back(3 - j);
		}
	}
	return v;
}

//undo a move
void GameField::undo() {
	int pastMove = pastMoves[pastMoves.size() - 1];
	pastMoves.pop_back();
	for (int i = 0; i < 6; i++) {
		if (field[i][pastMove] != " "){
			field[i][pastMove] = " ";
			break;
		}
	}
}

//opening book main function
int GameField::evaluateBook(std::string(*book)[67557][6][8], bool first) {
	int winLossDrawNone;
	winLossDrawNone = matchbook(book);
	switch (winLossDrawNone) {
	case 0:
		if (first) return 1000;
		else return -1000;
	case 1:
		if (first) return -1000;
		else return 1000;
	case 2:
		return 0;
	case -1:
		return -10000;
	}
	return -10000;
}

//check if board state is in the opening book
int GameField::matchbook(std::string(*book)[67557][6][8]) {
	bool next = false;
	for (int k = 0; k < 67557; k++) {
		for (int i = 5; i > -1; i--) {
			for (int j = 0; j < 7; j++) {
				if ((*book)[k][i][j] != field[i][j]) {
					next = true;
					break;
				}
			}
		}
		if (!next) {
			if ((*book)[k][0][7] == "win") {
			//	std::cout << "found a win" << std::endl;
				return 0;
			}

			if ((*book)[k][0][7] == "loss") {
		//		std::cout << "found a loss" << std::endl;
				return 1;
			}
			if ((*book)[k][0][7] == "draw") {
			//	std::cout << "found a draw" << std::endl;
				return 2;
			}
			return -1;
		}
		else {
			next = false;
		}

	}
}

//return number of moves played so far
int GameField::getMoveCount() {
	return pastMoves.size();
}