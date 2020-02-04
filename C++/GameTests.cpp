#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "Game.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <numeric>

using namespace std;

void runWithRedirectedConsoleOut(auto lambda, const string &fileName) {
	ofstream out(fileName);
	streambuf *coutbuf = std::cout.rdbuf();
	cout.rdbuf(out.rdbuf());

	lambda();

	std::cout.rdbuf(coutbuf); //reset to standard output again
}


void runFullGame() {
	bool notAWinner;
	Game aGame;

	aGame.add("Chet");
	aGame.add("Pat");
	aGame.add("Sue");

	do
	{
		aGame.roll(rand() % 5 + 1);

		if (rand() % 9 == 7)
		{
			notAWinner = aGame.wrongAnswer();
		}
		else
		{
			notAWinner = aGame.wasCorrectlyAnswered();
		}
	} while (notAWinner);
}

string fileName(const int index) {
	return "out/results/" + to_string(index) + ".txt";
}

TEST_CASE ("Golden Master") {
	for(int i = 0; i <= 10000; ++i) {
		srand(i);
		runWithRedirectedConsoleOut([]() { runFullGame(); }, fileName(i));
	}
}
