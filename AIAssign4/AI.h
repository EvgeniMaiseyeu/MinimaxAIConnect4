#pragma once
#include "GameField.h"
#include "BitboardField.h"
#include "TranspositionTable.h"

class AI {
private:
	std::string book[67557][6][8];    //IMPORTNAT: BOOK[x][0][7] IS TOP RIGHT CORNER POSITION WHICH WILL STORE WIN LOSS OR DRAW TEXT
	TranspositionTable* tt;
public:
	~AI();
	bool terminate;
	bool playFirst;
	AI(bool a, std::string fileName);
	int minimax(GameField* g, int depth, bool maxPlayer, int alpha, int beta);
	int makeMove(GameField* gf);
	void threadTest(std::vector<int> moves, std::vector<GameField*> bbs, int depth, std::vector<int> &v, int count);
	int bbminimax(BitboardField* bb, int depth, bool maxPlayer, int alpha, int beta, bool skipBook);
	int bbmakeMove(BitboardField* bb);
	void bbthreadTest(std::vector<int> moves, std::vector<BitboardField*> bbs, int depth, std::vector<int> &v, int count);
};