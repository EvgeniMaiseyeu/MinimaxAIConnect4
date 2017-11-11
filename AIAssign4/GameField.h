#pragma once
#include <string>
#include <vector>

class GameField {
private:
	std::string field[6][7];
	std::vector<int> pastMoves;
	int evaluationTable[6][7];
public:
	GameField();
	void printField();
	~GameField() {};
	bool gameOverCheck(std::string s);
	void addMove(int a, std::string p);
	bool validMove(std::string move);
	bool checkLeftDownDiag(int x, int y, std::string pat);
	bool checkRightDownDiag(int x, int y, std::string pat);
	int evaluate();
	std::vector<int> genMoves();
	void undo();
	bool draw();
	int getMoveCount();
	int evaluateBook(std::string(*book)[67557][6][8], bool playfirst);
	int matchbook(std::string(*book)[67557][6][8]);
};