#include "Game.h"
#include <cstdio>
#include <iostream>
#include <sstream>
#include <functional>
#include <numeric>

using namespace std;
using namespace std::placeholders;

auto createQuestionLambda = [](const string &prefix, const int index) -> string {
	return prefix + " " + to_string(index);
};

auto range = [](const int count) {
	vector<int> values(count);
	iota(begin(values), end(values), 0);
	return values;
};

template<typename Destination>
auto transformAll = [](const auto &source, const auto lambda) {
	Destination result;
	transform(source.begin(), source.end(), back_inserter(result), lambda);
	return result;
};

auto createQuestions = [](const int &count, const string &questionPrefix) {
	auto theRange = range(count);
	return transformAll<list<string>>(theRange, bind(createQuestionLambda, questionPrefix, _1));
};

auto create50Questions = bind(createQuestions, 50, _1);

auto oddRollLambda = [](const int roll) {
	return roll % 2 != 0;
};

auto advancePlaceLambda = [](const int oldPlace, const int roll) {
	int newPosition = oldPlace + roll;
	if (newPosition > 11) newPosition = newPosition - 12;
	return newPosition;
};

auto updatePlace = [](const vector<int> &places, const int currentPlayer, auto updateFunction) -> vector<int> {
	vector<int> newPlaces(places);
	newPlaces[currentPlayer] = updateFunction();
	return newPlaces;
};

auto categoryFromPlaceLambda(const int place) {
	if (place == 0) return "Pop";
	if (place == 4) return "Pop";
	if (place == 8) return "Pop";
	if (place == 1) return "Science";
	if (place == 5) return "Science";
	if (place == 9) return "Science";
	if (place == 2) return "Sports";
	if (place == 6) return "Sports";
	if (place == 10) return "Sports";
	return "Rock";
}

Game::Game() : currentPlayer(0), places(6), purses{} {
	popQuestions = create50Questions("Pop Question");
	scienceQuestions = create50Questions("Science Question");
	sportsQuestions = create50Questions("Sports Question");
	rockQuestions = create50Questions("Rock Question");
}

bool Game::isPlayable() {
	return (howManyPlayers() >= 2);
}

bool Game::add(string playerName) {
	players.push_back(playerName);
	places[howManyPlayers()] = 0;
	purses[howManyPlayers()] = 0;
	inPenaltyBox[howManyPlayers()] = false;

	cout << playerName << " was added" << endl;
	cout << "They are player number " << players.size() << endl;
	return true;
}

int Game::howManyPlayers() {
	return players.size();
}

void Game::roll(int roll) {
	cout << players[currentPlayer] << " is the current player" << endl;
	cout << "They have rolled a " << roll << endl;

	if (inPenaltyBox[currentPlayer]) {
		if (oddRollLambda(roll)) {
			doSomethingWhenInPenaltyBoxAndOddRoll(roll);
		} else {
			doSomethingWhenInPenaltyBoxAndEvenRoll();
		}
	} else {
		doSomethingWhenNotInPenaltyBox(roll);
	}
}

void Game::doSomethingWhenNotInPenaltyBox(int roll) {
	places = updatePlace(places, currentPlayer, bind(advancePlaceLambda, places[currentPlayer], roll));

	cout << players[currentPlayer] << "'s new location is " << places[currentPlayer] << endl;
	cout << "The category is " << currentCategory() << endl;
	askQuestion();
}

void Game::doSomethingWhenInPenaltyBoxAndEvenRoll() {
	cout << players[currentPlayer] << " is not getting out of the penalty box" << endl;
	isGettingOutOfPenaltyBox = false;
}

void Game::doSomethingWhenInPenaltyBoxAndOddRoll(int roll) {
	isGettingOutOfPenaltyBox = true;

	cout << players[currentPlayer] << " is getting out of the penalty box" << endl;
	places[currentPlayer] = places[currentPlayer] + roll;
	if (places[currentPlayer] > 11) places[currentPlayer] = places[currentPlayer] - 12;

	cout << players[currentPlayer] << "'s new location is " << places[currentPlayer] << endl;
	cout << "The category is " << currentCategory() << endl;
	askQuestion();
}

void Game::askQuestion() {
	string theCurrentCategory = categoryFromPlaceLambda(places[currentPlayer]);
	printAndRemovePopQuestion(theCurrentCategory);

	if (theCurrentCategory == "Science") {
		cout << scienceQuestions.front() << endl;
		scienceQuestions.pop_front();
	}
	if (theCurrentCategory == "Sports") {
		cout << sportsQuestions.front() << endl;
		sportsQuestions.pop_front();
	}
	if (theCurrentCategory == "Rock") {
		cout << rockQuestions.front() << endl;
		rockQuestions.pop_front();
	}
}

void Game::printAndRemovePopQuestion(const string &theCurrentCategory) {
	if (theCurrentCategory == "Pop") {
		cout << popQuestions.front() << endl;
		popQuestions.pop_front();
	}
}

string Game::currentCategory() {
	return categoryFromPlaceLambda(places[currentPlayer]);
}

bool Game::wasCorrectlyAnswered() {
	if (inPenaltyBox[currentPlayer]) {
		if (isGettingOutOfPenaltyBox) {
			cout << "Answer was correct!!!!" << endl;
			purses[currentPlayer]++;
			cout << players[currentPlayer]
			     << " now has "
			     << purses[currentPlayer]
			     << " Gold Coins." << endl;

			bool winner = didPlayerWin();
			currentPlayer++;
			if (currentPlayer == players.size()) currentPlayer = 0;

			return winner;
		} else {
			currentPlayer++;
			if (currentPlayer == players.size()) currentPlayer = 0;
			return true;
		}


	} else {

		cout << "Answer was corrent!!!!" << endl;
		purses[currentPlayer]++;
		cout << players[currentPlayer]
		     << " now has "
		     << purses[currentPlayer]
		     << " Gold Coins." << endl;

		bool winner = didPlayerWin();
		currentPlayer++;
		if (currentPlayer == players.size()) currentPlayer = 0;

		return winner;
	}
}

bool Game::wrongAnswer() {
	cout << "Question was incorrectly answered" << endl;
	cout << players[currentPlayer] + " was sent to the penalty box" << endl;
	inPenaltyBox[currentPlayer] = true;

	currentPlayer++;
	if (currentPlayer == players.size()) currentPlayer = 0;
	return true;
}


bool Game::didPlayerWin() {
	return !(purses[currentPlayer] == 6);
}
