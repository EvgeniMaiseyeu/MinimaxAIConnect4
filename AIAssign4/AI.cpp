

#include "stdafx.h"
#include "AI.h"
#include <algorithm>
#include <thread>
#include <fstream>
#include <mutex>

AI::AI(bool pFirst, std::string fileName) : playFirst(pFirst) {
	terminate = false;
	std::string line;
	std::ifstream myfile(fileName);
	int count = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			for (int i = 0; i < 83; i += 2) {
				if (line[i] == 'b')
					book[count][5 - (i / 2) % 6][(i / 2) / 6] = " ";
				else
					book[count][5 - (i / 2) % 6][(i / 2) / 6] += line[i];
				/*
				else if (line[i] == 'x')
				book[count][5 - (i / 2) % 6][(i / 2) / 6] = "x";
				else if (line[i] == 'o')
				book[count][5 - (i / 2) % 6][(i / 2) / 6] = "o";
				*/
			}
			if (line.length() == 87)
				book[count][0][7] = "win";
			else if (line[86] == 's')
				book[count][0][7] = "loss";
			else
				book[count][0][7] = "draw";

			//	std::cout << count << std::endl;
			count++;
		}
		myfile.close();
	}
}

//(*the minimax value of n, searched to depth d *)
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

void AI::threadTest(std::vector<int> moves, std::vector<GameField*> gs, int depth, std::vector<int> &v, int count) {
	int value = -1000;
	int oldvalue = 0;
	gs[count]->addMove(moves[count], "x");
	value = std::max(value, minimax(gs[count], depth - 1, false, -10000, 10000));
	gs[count]->undo();
	v[count] = value;
	//store value;
}

int AI::bbminimax(BitboardField * bb, int depth, bool maxPlayer, int alpha, int beta, bool skipBook)
{
	if (this->terminate) {
		return 0;
	}
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
			value = std::max(value, bbminimax(bb, depth - 1, !maxPlayer, alpha, beta, skipBook));
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
			value = std::min(value, bbminimax(bb, depth - 1, !maxPlayer, alpha, beta, skipBook));
			bb->undo();
			beta = std::min(beta, value);
			if (beta <= alpha)
				break;
		}
		return value;
	}

}

int AI::bbmakeMove(BitboardField * bb)
{
	int depth = 8;
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
				playFirst ? depth = 14 : depth = 14;
			}
			else {
				depth = 25;
			}

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

void AI::bbthreadTest(std::vector<int> moves, std::vector<BitboardField*> bbs, int depth, std::vector<int>& v, int count)
{
	int value = -1000;
	int oldvalue = 0;
	bbs[count]->aiAddMove(moves[count]);
	value = std::max(value, bbminimax(bbs[count], depth - 1, false, -10000, 10000, false));
	bbs[count]->undo();
	v[count] = value;
	//store value;
}
