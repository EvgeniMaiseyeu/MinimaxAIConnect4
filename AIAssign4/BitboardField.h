#pragma once
#include <string>
#include <vector>

class BitboardField {
private:
	unsigned __int64 aiBoard;
	unsigned __int64 playerBoard;
	unsigned __int64 blankBoard = 0b0000000000000000000000000000000000000000000000000000000000000000;
	unsigned __int64 fullBoard = 0b1111111111111111111111111111111111111111111111111111111111111111;
	std::string field[6][7];
	std::vector<unsigned __int64> book;
	std::vector<std::string> bookResults;
	std::vector<int> pastMoves;
	int evaluationTable[6][7];
public:
	BitboardField();
	BitboardField(std::vector<unsigned __int64> openings);
	BitboardField(std::string fileName);
	void printField();
	bool playerWonCheck();
	bool aiWonCheck();
	void aiAddMove(int a);
	void playerAddMove(int a);
	bool validMove(std::string move);
	int evaluate();
	std::vector<int> genMoves();
	void undo();
	bool draw();
	int getMoveCount();
	int evaluateBook(bool playfirst);
	void convertIntoArray();
};