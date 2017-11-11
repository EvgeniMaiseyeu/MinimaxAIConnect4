#pragma once
#include <string>
#include <vector>

class BitboardField {
private:
	unsigned __int64 aiBoard;
	unsigned __int64 playerBoard;
	std::vector<unsigned __int64> book;
	std::vector<std::string> bookResults;
	std::vector<int> pastMoves;
	int evaluationTable[6][7];
public:
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
};