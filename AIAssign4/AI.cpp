

#include "stdafx.h"
#include "AI.h"
#include <algorithm>
#include <thread>
#include <fstream>
#include <mutex>

//clears memory used by AI class
AI::~AI()
{
	//delete(tt);
}

//constructor for ai
AI::AI(bool pFirst, std::string fileName) : playFirst(pFirst) {
	//tt = new TranspositionTable();
	terminate = false;
}

//minimax for old game field not  used
int AI::minimax(GameField* g, int depth, bool maxPlayer, int alpha, int beta) {
	if (this->terminate) {
		return 0;
	} else if (g->getMoveCount() < 9 && g->getMoveCount() > 9/*opening book 8 ply*/) {
			//depth = 8 - g->getMoveCount();
		if (g->gameOverCheck("x")) {
			return 1000;
		}
		else if (g->gameOverCheck("o")) {
			return -1000;
		}
		else if (depth == 0) {
			std::string(*qq)[67557][6][8] = &book;
			int z = g->evaluateBook(qq, playFirst);
			if (z == -10000) {
				depth = 1;
			}
			else
				return z;
		}
	}
	else if (g->gameOverCheck("x")) {
		return 1000;
	}
	else if (g->gameOverCheck("o")) {
		return -1000;
	}
	else if (depth == 0) {
		return g->evaluate();
	}

	std::vector<int> moves = g->genMoves();
	if (maxPlayer) {
		int value = -1000;
		for (int i = 0; i < moves.size(); i++) {
			g->addMove(moves[i], "x");
			value = std::max(value, minimax(g, depth - 1, !maxPlayer, alpha, beta));
			alpha = std::max(alpha, value);
			g->undo();
			if (beta <= alpha)
				break;
		}
		return value;
	}
	else {
		int value = 1000;
		for (int i = 0; i < moves.size(); i++) {
			g->addMove(moves[i], "o");
			value = std::min(value, minimax(g, depth - 1, !maxPlayer, alpha, beta));
			g->undo();
			beta = std::min(beta, value);
			if (beta <= alpha)
				break;
		}
		return value;
	}

}

//controls the minimax calls
int AI::makeMove(GameField* gf) {
	int depth = 2;
	int moveIndex = 0;
	int oldvalue = 0;
	std::vector<int> moves = gf->genMoves();
	std::vector<std::thread> threads;
	std::vector<GameField*> gs;
	std::vector<int> vals;


	// game boards for threads to eval
	for (int i = 0; i < moves.size(); i++) {
		GameField *g = new GameField();
		int z = 10;
		vals.push_back(z);
		*g = *gf;
		gs.push_back(g);
	}
	bool win = false;
	if (gf->getMoveCount() > 8) {
		int value = -1000;
		for (int i = 0; i < moves.size(); i++) {
			gf->addMove(moves[i], "x");
			value = minimax(gf, depth - 1, false, -10000, 10000);
			gf->undo();
			vals[i] = value;
			if (value == 1000) {
				win = true;
				break;
			}
		}
	}

	//create threads
	//launch threads
	std::mutex m;

	moveIndex = max_element(vals.begin(), vals.end()) - vals.begin();
	if (!win) {
		bool forced = false;
		int fCount = 0;
		for (int i = 0; i < vals.size(); i++) {
			if (vals[i] == -1000)
				fCount++;
			if (fCount == vals.size()-1) {
				forced = true;
				break;
			}
		}

		if (!forced) {
		
			if (moves.size() == 7) {
				//(42 needed worst case)
				depth = 10;
			}
			else if (moves.size() == 6) {
				//(36 needed worst case)
				depth = 11;
			}
			else if (moves.size() == 5) {
				//(30 needed worst case)
				depth = 13;
			}
			else {
				depth = 25;
			}

			for (int i = 0; i < moves.size(); i++) {
				threads.push_back(std::thread(&AI::threadTest, this, std::ref(moves), std::ref(gs), depth, std::ref(vals), i));
				m.lock();
				for (int j = 0; j < vals.size(); j++) {
					if (vals[j] == 1000) {
						terminate = true;
						break;
					}
				}
				m.unlock();
			}

			//sync threads
			for (auto& th : threads) {
				th.join();
				//th.~thread();
			}
			terminate = false;
		}
		moveIndex = max_element(vals.begin(), vals.end()) - vals.begin();
	}
	//delete boards created for testing
	for (int i = 0; i < moves.size(); i++) {
		delete(gs[i]);
	}
	gf->addMove(moves[moveIndex], "x");
	return moves[moveIndex];
}

//old thread function  to split up minimax work
void AI::threadTest(std::vector<int> moves, std::vector<GameField*> gs, int depth, std::vector<int> &v, int count) {
	int value = -1000;
	int oldvalue = 0;
	gs[count]->addMove(moves[count], "x");
	value = std::max(value, minimax(gs[count], depth - 1, false, -10000, 10000));
	gs[count]->undo();
	v[count] = value;
	//store value;
}

//current bitboard minimax algorithm with optimizations
int AI::bbminimax(BitboardField * bb, int depth, bool maxPlayer, int alpha, int beta, bool skipBook)
{
	if (this->terminate) {
		return 0;
	}
	/*
	unsigned __int64 pB = bb->getPlayerBoard();
	unsigned __int64 aiB = bb->getAIBoard();
	unsigned __int16 ttScore = tt->get(pB, aiB);
	if (ttScore != 0) {
		tpcount++;
		if (ttScore == 10001) return -1000;
		else return ttScore;
	}
	*/
	else if (bb->getMoveCount() < 9 && !skipBook && playFirst/*opening book 8 ply*/) {
		depth = 8 - bb->getMoveCount();
		if (bb->aiWonCheck()) {
			return 1000;
		}
		else if (bb->playerWonCheck()) {
			return -1000;
		}
		else if (depth == 0) {
			int z = bb->evaluateBook(playFirst);
			if (z == -10000) {
				depth = 1;
			}
			else
				return z;
		}
	}
	else if (bb->aiWonCheck()) {
		return 1000;
	}
	else if (bb->playerWonCheck()) {
		return -1000;
	}
	else if (depth == 0) {
		return bb->evaluate();
	}

	std::vector<int> moves = bb->genMoves();
	if (maxPlayer) {
		int value = -1000;
		for (int i = 0; i < moves.size(); i++) {
			bb->aiAddMove(moves[i]);
			bb->convertIntoArray();
			value = std::max(value, bbminimax(bb, depth - 1, !maxPlayer, alpha, beta, skipBook));
			/*if (ttScore == 0 && !skipBook) {
				unsigned __int16 val = 0;
				if (value == -1000)
					val = 10001;
				else
					val = value;
				tt->store(pB, aiB, val);
			}*/
			alpha = std::max(alpha, value);
			bb->undo();
			if (beta <= alpha)
				break;
		}

		return value;
	}
	else {
		int value = 1000;
		for (int i = 0; i < moves.size(); i++) {
			bb->playerAddMove(moves[i]);
			bb->convertIntoArray();
			value = std::min(value, bbminimax(bb, depth - 1, !maxPlayer, alpha, beta, skipBook));
		/*	if (ttScore == 0 && !skipBook) {
				unsigned __int16 val = 0;
				if (value == -1000)
					val = 10001;
				else
					val = value;
				tt->store(pB, aiB, val);
			}*/
			bb->undo();
			beta = std::min(beta, value);
			if (beta <= alpha)
				break;
		}

		return value;
	}

}

// minimax controller can set depth and starts threads and waits for them to join
int AI::bbmakeMove(BitboardField * bb)
{
	int depth = 4;
	int moveIndex = 0;
	int oldvalue = 0;
	std::vector<int> moves = bb->genMoves();
	std::vector<std::thread> threads;
	std::vector<BitboardField*> bbs;
	std::vector<int> vals;


	// game boards for threads to eval
	for (int i = 0; i < moves.size(); i++) {
		BitboardField *b = new BitboardField();
		int z = 10;
		vals.push_back(z);
		*b = *bb;
		bbs.push_back(b);
	}
	bool win = false;
	if (bb->getMoveCount() > 0) {
		int value = -1000;
		for (int i = 0; i < moves.size(); i++) {
			bb->aiAddMove(moves[i]);
			value = bbminimax(bb, depth - 1, false, -10000, 10000, true);
			bb->undo();
			vals[i] = value;
			if (value == 1000) {
				win = true;
				break;
			}
		}
	}

	//create threads
	//launch threads
	std::mutex m;

	moveIndex = max_element(vals.begin(), vals.end()) - vals.begin();
	
	if (!win) {
		bool forced = false;
		int fCount = 0;
		for (int i = 0; i < vals.size(); i++) {
			if (vals[i] == -1000)
				fCount++;
			if (fCount == vals.size() - 1) {
				forced = true;
				break;
			}
		}

		if (!forced) {

			if (moves.size() == 7) {
				//(42 needed worst case)
				playFirst ? depth = 11 : depth = 11;
			}
			else if (moves.size() == 6) {
				//(36 needed worst case)
				playFirst ? depth = 12 : depth = 12;
			}
			else if (moves.size() == 5) {
				//(30 needed worst case)
				playFirst ? depth = 15 : depth = 14;
			}
			else {
				depth = 25;
			}
			printf("%d\n", moves.size());
			for (int i = 0; i < moves.size(); i++) {
				threads.push_back(std::thread(&AI::bbthreadTest, this, std::ref(moves), std::ref(bbs), depth, std::ref(vals), i));
				m.lock();
				for (int j = 0; j < vals.size(); j++) {
					if (vals[j] == 1000) {
						terminate = true;
						break;
					}
				}
				m.unlock();
			}

			//sync threads
			for (auto& th : threads) {
				th.join();
				//th.~thread();
			}
			terminate = false;
		}
		moveIndex = max_element(vals.begin(), vals.end()) - vals.begin();
	}
	
	//delete boards created for testing
	for (int i = 0; i < moves.size(); i++) {
		delete(bbs[i]);
	}
	bb->aiAddMove(moves[moveIndex]);
	return moves[moveIndex];
}

//current bitboard thread function each thread calls this to check a possible move subtree and return the value
void AI::bbthreadTest(std::vector<int> moves, std::vector<BitboardField*> bbs, int depth, std::vector<int>& v, int count)
{
	int value = -1000;
	int oldvalue = 0;
	bbs[count]->aiAddMove(moves[count]);
	bbs[count]->convertIntoArray();
	value = std::max(value, bbminimax(bbs[count], depth - 1, false, -10000, 10000, false));
	bbs[count]->undo();
	v[count] = value;
	//store value;
}
