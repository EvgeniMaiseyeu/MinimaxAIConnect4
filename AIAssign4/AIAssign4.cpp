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
	//create AI
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



	//create game field
	//GameField* gameField = new GameField();


	//if AI first it makes a move
	if (s == "n") {
	//	ai->makeMove(gameField);
		ai->bbmakeMove(bbf);
		moveCount++;
	}

	//draw field state before players first move
	//gameField->printField();
	bbf->printField();

	//handle gameplay
	string line;
	while (getline(cin, line))
	{
		if (line == "undo") {
		//	gameField->undo();
			//gameField->undo();
			bbf->undo();
			bbf->undo();
		//	gameField->printField();
			bbf->printField();
		}
		else {
			/*
			cout << "starting game field play" << endl;
			//if valid move
			if (gameField->validMove(line)) {
				gameField->addMove((line[0] - '0'), "o");
				gameField->printField();
				moveCount++;

				//check if player won
				if (gameField->gameOverCheck("o")) {
					cout << "Congrats! You Won!" << endl;
					break;
				}
				if (gameField->draw()) {
					cout << "Game is a draw -_-" << endl;
					break;
				}
				std::clock_t time;

				time = std::clock();

				//time = start;
				int a = ai->makeMove(gameField);
				//time = stop;

				printf("AI moved in column %d\n", a);
				printf("Traversal time: %f milliseconds\n", (std::clock() - time) / (double)(CLOCKS_PER_SEC / 1000));

				gameField->printField();
				moveCount++;


				//check if AI won
				if (gameField->gameOverCheck("x")) {
					cout << "The AI has triumphed! Better luck next time" << endl;
					break;
				}
				if (gameField->draw()) {
					cout << "Game is a draw -_-" << endl;
					break;
				}

			}
			else {
				cout << "invalid move you buffoon, try again!" << endl;
			}
			*/
		//	cout << "starting bb play" << endl;
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
//	delete(gameField);

    return 0;
}