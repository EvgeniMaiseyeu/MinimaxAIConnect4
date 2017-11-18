#pragma once
#include "BitboardField.h"
#include <thread>
#include "TranspositionTable.h"

class AI {
private:
	TranspositionTable* tt;
	int tpcount;
public:
	~AI();
	bool terminate;
	std::vector<BitboardField*> bbsIdle;
	bool playFirst;
	AI(bool a, std::string fileName);
	int bbminimax(BitboardField* bb, int depth, bool maxPlayer, int alpha, int beta, bool skipBook);
	int bbmakeMove(BitboardField* bb);
	void bbthreadTest(std::vector<int> moves, std::vector<BitboardField*> bbs, int depth, std::vector<int> &v, int count, bool skipBook);
	void idleComputation(std::vector<int> moves, int depth, std::vector<int> &v, int count);
	std::vector<std::thread> generateTranspositions(BitboardField * bb);
};