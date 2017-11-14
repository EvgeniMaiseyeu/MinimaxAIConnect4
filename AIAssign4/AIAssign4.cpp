// AIAssign4.cpp : Defines the entry point for the console application.
//
#pragma once
#include "stdafx.h"
#include "GameField.h"
#include "AI.h"
#include <string>
#include <ctime>
using namespace std;

int main()
{
	//init variables
	int moveCount = 0;
	bool aiPlayFirst = false;
	//create game board
	BitboardField* bbf = new BitboardField("connect4.txt");

	//create AI
	AI* ai = new AI(aiPlayFirst, "connect4.txt");

	//intro messages
	cout << "Hello challenger, do you want to play first? (y/n)" << endl;
	string s;
	while (getline(cin, s)) {
		if (s == "y")
			break;
		if (s == "n") {
			ai->playFirst = true;
			break;
		}
	}

	//if AI first it makes a move
	if (s == "n") {
		ai->bbmakeMove(bbf);
		moveCount++;
	}

	//draw field state before players first move
	bbf->printField();

	//handle gameplay
	string line;
	while (getline(cin, line))
	{
		if (line == "undo") {
			bbf->undo();
			bbf->undo();
			bbf->printField();
		}
		else {
			//if valid move
			if (bbf->validMove(line)) {
				bbf->playerAddMove((line[0] - '0'));
				bbf->printField();
				moveCount++;

				//check if player won
				if (bbf->playerWonCheck()) {
					cout << "Congrats! You Won!" << endl;
					break;
				}
				if (bbf->draw()) {
					cout << "Game is a draw -_-" << endl;
					break;
				}
				std::clock_t time;

				time = std::clock();

				int a = ai->bbmakeMove(bbf);

				printf("AI moved in column %d\n", a);
				printf("Traversal time: %f milliseconds\n", (std::clock() - time) / (double)(CLOCKS_PER_SEC / 1000));

				bbf->printField();
				moveCount++;


				//check if AI won
				if (bbf->aiWonCheck()) {
					cout << "The AI has triumphed! Better luck next time" << endl;
					break;
				}
				if (bbf->draw()) {
					cout << "Game is a draw -_-" << endl;
					break;
				}

			}
			else {
				cout << "invalid move you buffoon, try again!" << endl;
			}
			
		}

	};

	std::cout << "Type something and hit enter to close:" << endl;
	std::cin >> s;
	//game over clear memory
	delete(ai);
	delete(bbf);

    return 0;
}