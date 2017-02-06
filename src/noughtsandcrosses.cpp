#include "noughtsandcrosses.h"
#include <vector>
#include <iostream>


int Player::getMark() const {
	return mark;
}

int Player::getScore() const {
	return score;
}

Player::Player(Player::PlayerMark playersMark) : mark(playersMark), score(0) {
}

void Field::setMark(Player::PlayerMark mark) {
	this->mark = mark;
	emit markChanged();
}

int Field::getMark() const {
	return mark;
}

Field::Field() : mark(0) {
}

NoughtsAndCrosses::NoughtsAndCrosses(QObject *parent) : QObject(parent), state(GameState::Start) {
	player1 = new Player(Player::Nought);
	player2 = new Player(Player::Cross);
	currentPlayer = player1;

	for (auto i = 0; i < 9; i++) {
		auto field = new Field();
		map.append(field);
	}
}

NoughtsAndCrosses::~NoughtsAndCrosses() {
	for (auto i = 0; i < map.size(); i++) {
		delete map.takeLast();
	}

	delete player1;
	delete player2;
}

/* Called whenever a player marks a field.
 * Checks all possible lines to determin if current player won.
 */
void NoughtsAndCrosses::check() {
	std::cout << "checking" << std::endl;

	// Local variable needed to capture `map` in lambdas below
	auto map = this->map;

	// Convenience helper to easily get mark of field under index `fieldId`
	auto getMark = [&map] (int fieldId) {
		auto field = static_cast<Field*>(map.at(fieldId));
		return static_cast<Player::PlayerMark>(field->getMark());
	};

	// Checks one line
	// `first` is index of first field to check
	// `displacement` defines consecutive index difference in a given line
	auto checkLine = [&getMark] (int first, int displacement) {
		if (getMark(first) != Player::None
			&& getMark(first) == getMark(first + displacement)
			&& getMark(first) == getMark(first + displacement * 2)) {
			return WinCondition(first, getMark(first));
		}
		return WinCondition(-1, Player::None);
	};

	// Checks three different lines
	// `gap` defines first indexes of checked lines
	// `displacement` defines consecutive index difference in a given line
	auto checkLines = [&checkLine] (int gap, int displacement) {
		for (int first : {0, gap, 2 * gap}) {
			auto win = checkLine(first, displacement);
			if (win.first != -1) {
				return win;
			}
		}
		return WinCondition(-1, Player::None);
	};

//	int gap = 1, displacement = 3;
//	for (int first : {0, gap, 2 * gap}) {
//		if (getMark(first) != Player::None
//			&& getMark(first) == getMark(first + displacement)
//			&& getMark(first) == getMark(first + displacement * 2)) {
//			std::cout << "yes! " << first << std::endl;
//		}
//	}

	// check vertical
	auto verticalWin = checkLines(1, 3);

	// check horizontal
	auto horizontalWin = checkLines(3, 1);

	// check diagonals
	auto leftDiagonalWin = checkLine(0, 4);
	auto rightDiagonalWin = checkLine(2, 3);

	auto printWin = [] (WinCondition winCondition) {
		std::cout << "win left? " << winCondition.first << " player " << winCondition.second << std::endl;
	};

	if (verticalWin.first != -1) {
		printWin(verticalWin);
		setState(GameState::End);
	}
	if (horizontalWin.first != -1) {
		printWin(horizontalWin);
	}
	if (leftDiagonalWin.first != -1) {
		printWin(leftDiagonalWin);
	}
	if (rightDiagonalWin.first != -1) {
		printWin(rightDiagonalWin);
	}


}

QList<QObject*> NoughtsAndCrosses::getMap () const {
	return map;
}

void NoughtsAndCrosses::setState(NoughtsAndCrosses::GameState newState)
{
	if (state != newState) {
		state = newState;
		emit stateChanged();
	}
}

void NoughtsAndCrosses::markField(const int fieldId) {
	auto field = static_cast<Field*>(map.at(fieldId));

	setState(GameState::Playing);

	// when field is empty mark it as current player's field, check win conditions, and change player
	if (field->getMark() == Player::None) {
		field->setMark(static_cast<Player::PlayerMark>(currentPlayer->getMark()));
		check();
		changePlayer();
	}
	else {
		return;
	}
}

void NoughtsAndCrosses::startGame()
{
	setState(GameState::Start);
}

QObject *NoughtsAndCrosses::getCurrentPlayer () const {
	return currentPlayer;
}

QObject *NoughtsAndCrosses::getPlayer1() const {
	return player1;
}

QObject *NoughtsAndCrosses::getPlayer2() const {
	return player2;
}

int NoughtsAndCrosses::getState() const
{
	return state;
}

QList<QObject *> NoughtsAndCrosses::getWinSequences() const
{
	return winSequences;
}

void NoughtsAndCrosses::changePlayer() {
	if (state == GameState::Playing) {
		currentPlayer = (player1 == currentPlayer) ? player2 : player1;
		emit currentPlayerChanged();
	}
}
