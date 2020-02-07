#include <iostream>
#include <list>
#include <vector>
using namespace std;

#ifndef GAME_H_
#define GAME_H_

class Game{

		private:
			vector<string> players;

			vector<int> places;
			int purses[6];

			bool inPenaltyBox[6];

			list<string> popQuestions;
			list<string> scienceQuestions;
			list<string> sportsQuestions;
			list<string> rockQuestions;

			int currentPlayer;
			bool isGettingOutOfPenaltyBox;

public:
	Game();

	bool isPlayable();
	bool add(string playerName);

	int howManyPlayers();
	void roll(int roll);

	private:
		void askQuestion();
		string currentCategory();

				public:
					bool wasCorrectlyAnswered();
					bool wrongAnswer();

private:
	bool didPlayerWin();

	void doSomethingWhenInPenaltyBoxAndOddRoll(int roll);

	void doSomethingWhenInPenaltyBoxAndEvenRoll();

	void doSomethingWhenNotInPenaltyBox(int roll);

	void printAndRemovePopQuestion(const string &theCurrentCategory);
};

#endif /* GAME_H_ */
