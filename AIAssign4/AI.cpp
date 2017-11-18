

#include "stdafx.h"
#include "AI.h"
#include <algorithm>
#include <thread>
#include <fstream>
#include <mutex>

//clears memory used by AI class
AI::~AI()
{
	delete(tt);
}

//constructor for ai
AI::AI(bool pFirst, std::string fileName) : playFirst(pFirst) {
	tt = new TranspositionTable();
	terminate = false;
}

//---------------------------------------------
//Bit board minimax
//---------------------------------------------
//current bitboard minimax algorithm with optimizations
int AI::bbminimax(BitboardField * bb, int depth, bool maxPlayer, int alpha, int beta, bool skipBook)
{
	if (this->terminate) {
		return 0;
	}
	int alphaOriginal = alpha;
	int betaOriginal = beta;
	int moveOrder = 7;
	bool negated = false;
	unsigned __int64 pB = bb->getPlayerBoard();
	unsigned __int64 aiB = bb->getAIBoard();
	unsigned __int32 ttScore = tt->get(pB, aiB);
	if (bb->aiWonCheck()) {
		return 1000;
	}
	else if (bb->playerWonCheck()) {
		return -1000;
	}
	else if (bb->getMoveCount() > 40)
		return 0;
	if (bb->getMoveCount() < 9 && !skipBook/*opening book 8 ply*/) {
		depth = 8 - bb->getMoveCount();

		if (depth == 0) {
			int z = bb->evaluateBook(playFirst);
			if (z == -10000) {
				depth = 6;
			}
			else
				return z;
		}
	}
	else {
		if (ttScore != 0 && depth <= (ttScore >> 16)) {
			unsigned __int32 scoreMask = 0b00000000000000000000001111111111;
			unsigned __int32 negMask = 1;
			unsigned __int32 typeMask = 3;
			tpcount++;
			if ((ttScore >> 15) & negMask) {
				return -1000;
			}
			else {
				if ((ttScore & typeMask) == 0)
					return (ttScore >> 5 & scoreMask);
				else if ((ttScore & typeMask) == 1)
					alpha = std::max(alpha, (int)(ttScore >> 5 & scoreMask));
				else if ((ttScore & typeMask) == 2)
					beta = std::min(beta, (int)(ttScore >> 5 & scoreMask));
				if (alpha > beta)
					return((ttScore >> 5) & scoreMask);
			}
		}
		else if (ttScore != 0) {
			unsigned __int32 moveMask = 0b00000000000000000000000000000111;
			moveOrder = moveMask & (ttScore >> 2);
		}
	}
	if (depth == 0) {
		return bb->evaluate();
	}
	std::vector<int> moves = bb->genMoves();
	if (moveOrder != 7 && moves.size() > 1) {
		std::vector<int>::iterator it;
		it = find(moves.begin(), moves.end(), moveOrder);
		int index = distance(moves.begin(), it);
		if (index != 0) {
			std::swap(moves[0], moves[index-1]);
		}
	}
	if (maxPlayer) {
		int value = -1000;
		int i = 0;
		for (i = 0; i < moves.size(); i++) {
			bb->aiAddMove(moves[i]);
		//	bb->convertIntoArray();
			value = std::max(value, bbminimax(bb, depth - 1, !maxPlayer, alpha, beta, skipBook));

			alpha = std::max(alpha, value);
			bb->undo();
			if (beta <= alpha)
				break;
		}
		unsigned __int32 hashValue = 0;
		unsigned __int32 hashmove = 0; 
		unsigned __int32 hashneg = 0;
		unsigned __int32 hashtype = 0;
		unsigned __int32 hashdepth = 0;
		if (value < 0) {
			negated = true;
			value = -value;
			hashneg = 1;
		}
		if (i < moves.size()) {
			hashmove = i;
		}
		else {
			hashmove = 7;
		}
		hashValue = value;
		if (value <= alphaOriginal)
			hashtype = 2;
		else if (value >= betaOriginal)
			hashtype = 1;
		else
			hashtype = 0;

		hashdepth = depth;
		hashValue = hashtype | (hashmove << 2) | (hashValue << 5) | (hashneg << 15) | (hashdepth << 16);
		tt->store(pB, aiB, hashValue);
		if (negated) {
			value = -value;
			negated = false;
		}
		return value;
	}
	else {
		int value = 1000;
		int i = 0;
		for (i = 0; i < moves.size(); i++) {
			bb->playerAddMove(moves[i]);
		//	bb->convertIntoArray();
			value = std::min(value, bbminimax(bb, depth - 1, !maxPlayer, alpha, beta, skipBook));

			bb->undo();
			beta = std::min(beta, value);
			if (beta <= alpha)
				break;
		}

		unsigned __int32 hashValue = 0;
		unsigned __int32 hashmove = 0;
		unsigned __int32 hashneg = 0;
		unsigned __int32 hashtype = 0;
		unsigned __int32 hashdepth = 0;
		if (value < 0) {
			negated = true;
			value = -value;
			hashneg = 1;
		}
		if (i < moves.size()) {
			hashmove = i;
		}
		else {
			hashmove = 7;
		}
		hashValue = value;
		if (value <= alphaOriginal)
			hashtype = 2;
		else if (value >= betaOriginal)
			hashtype = 1;
		else
			hashtype = 0;

		hashdepth = depth;
		hashValue = hashtype | (hashmove << 2) | (hashValue << 5) | (hashneg << 15) | (hashdepth << 16);
		tt->store(pB, aiB, hashValue);
		if (negated) {
			value = -value;
			negated = false;
		}
		return value;
	}

}

// minimax controller can set depth and starts threads and waits for them to join
int AI::bbmakeMove(BitboardField * bb)
{
	int depth = 3;
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
			/*
			if (moves.size() == 7) {
				//(42 needed worst case)
				playFirst ? depth = 11 + bb->getMoveCount()/3: depth = 11 + bb->getMoveCount()/3;
			}
			else if (moves.size() == 6) {
				//(36 needed worst case)
				playFirst ? depth = 13 + bb->getMoveCount() / 3 : depth = 13 + bb->getMoveCount() / 3;
			}
			else if (moves.size() == 5) {
				//(30 needed worst case)
				playFirst ? depth = 30 : depth = 30;
			}
			else {
				depth = 25;
			}*/
			if (bb->getMoveCount() < 12) {
				//(42 needed worst case)
				playFirst ? depth = 11 + bb->getMoveCount() / 2 : depth = 11 + bb->getMoveCount() / 2;
			}
			else if (bb->getMoveCount() < 18) {
				//(36 needed worst case)
				playFirst ? depth = 12 + bb->getMoveCount() / 3 : depth = 12 + bb->getMoveCount() / 3;
			}
			else if (bb->getMoveCount() < 22) {
				//(30 needed worst case)
				playFirst ? depth = 18 + bb->getMoveCount() / 2 : depth = 18 + bb->getMoveCount() / 2;
			}
			else {
				depth = 25;
			}
			threads.clear();
			for (int i = 0; i < moves.size(); i++) {
				threads.push_back(std::thread(&AI::bbthreadTest, this, std::ref(moves), std::ref(bbs), depth, std::ref(vals), i, false));
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
			// if all values are -1000 and total moves less then 9 and we are 2nd redo without opening book

			if (!playFirst && bb->getMoveCount() < 9) {
				bool skip = false;
				for (int i = 0; i < vals.size(); i++) {
					if (vals[i] != -1000)
						skip = true;
						break;
				}
				if (*max_element(vals.begin(), vals.end()) == 1000) {
					skip = true;
				}
				if (!skip) {
					threads.clear();
					for (int i = 0; i < moves.size(); i++) {
						threads.push_back(std::thread(&AI::bbthreadTest, this, std::ref(moves), std::ref(bbs), depth, std::ref(vals), i, true));
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
				}
			}

			terminate = false;
		}
		moveIndex = max_element(vals.begin(), vals.end()) - vals.begin();
	}
	printf("%d\n", tpcount);
	//delete boards created for testing
	for (int i = 0; i < moves.size(); i++) {
		delete(bbs[i]);
	}
	bb->aiAddMove(moves[moveIndex]);
	return moves[moveIndex];
}

//current bitboard thread function each thread calls this to check a possible move subtree and return the value
void AI::bbthreadTest(std::vector<int> moves, std::vector<BitboardField*> bbs, int depth, std::vector<int>& v, int count, bool skipBook)
{
	int value = -1000;
	int oldvalue = 0;
	bbs[count]->aiAddMove(moves[count]);
//	bbs[count]->convertIntoArray();
	value = std::max(value, bbminimax(bbs[count], depth - 1, false, -10000, 10000, skipBook));
	bbs[count]->undo();
	v[count] = value;
	//store value;
}


//----------------------------------------
//Think on opponent turn
//----------------------------------------
void AI::idleComputation(std::vector<int> moves, int depth, int count)
{
	int value = 1000;
	this->bbsIdle[count]->playerAddMove(moves[count]);
	value = std::min(value, bbminimax(this->bbsIdle[count], depth - 1, true, -10000, 10000, true));
	this->bbsIdle[count]->undo();
	valsIdle[count] = value;
}

std::vector<std::thread> AI::generateTranspositions(BitboardField * bb)
{
	int depth = 0;
	movesIdle = bb->genMoves();
	std::vector<std::thread> threads;

	std::vector<int> vals;
	valsIdle.clear();
	bbsIdle.clear();
	// game boards for threads to eval
	for (int i = 0; i < movesIdle.size(); i++) {
		BitboardField *b = new BitboardField();
		valsIdle.push_back(-1);
		*b = *bb;
		bbsIdle.push_back(b);
	}
	/*
	if (bb->getMoveCount() < 12) {
		//(42 needed worst case)
		playFirst ? depth = 20 + bb->getMoveCount() / 3 : depth = 20 + bb->getMoveCount() / 3;
	}
	else if (bb->getMoveCount() < 18) {
		//(36 needed worst case)
		playFirst ? depth = 21 + bb->getMoveCount() / 3 : depth = 21 + bb->getMoveCount() / 3;
	}
	else if (bb->getMoveCount() < 22) {
		//(30 needed worst case)
		playFirst ? depth = 25 + bb->getMoveCount() / 3 : depth = 25 + bb->getMoveCount() / 3;
	}
	else {
		depth = 32;
	}*/
	if (bb->getMoveCount() < 12) {
		//(42 needed worst case)
		playFirst ? depth = 20 + bb->getMoveCount() / 2 : depth = 20 + bb->getMoveCount() / 2;
	}
	else if (bb->getMoveCount() < 18) {
		//(36 needed worst case)
		playFirst ? depth = 21 + bb->getMoveCount() / 3 : depth = 21 + bb->getMoveCount() / 3;
	}
	else if (bb->getMoveCount() < 22) {
		//(30 needed worst case)
		playFirst ? depth = 25 + bb->getMoveCount() / 3 : depth = 25 + bb->getMoveCount() / 3;
	}
	else {
		depth = 32;
	}
	
	for (int i = 0; i < movesIdle.size(); i++) {
		threads.push_back(std::thread(&AI::idleComputation, this, std::ref(movesIdle), depth, i));
	}

	return threads;

}

