#include "noughtsandcrosses.h"
#include <vector>
#include <iostream>


int Player::getMark() const {
	return mark;
}

int Player::getScore() const {
	return score;
}

int Player::getIndex() const
{
	return index;
}

void Player::wonGame()
{
	score++;
	emit scoreChanged(score);
}

Player::Player(Player::PlayerMark playersMark, int setIndex) : mark(playersMark), score(0), index(setIndex) {
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

NoughtsAndCrosses::NoughtsAndCrosses(QObject *parent) : QObject(parent), winner(nullptr), state(GameState::Start) {
	player1 = new Player(Player::Nought, 1);
	player2 = new Player(Player::Cross, 2);
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

	for (auto i = 0; i < winSequences.size(); i++) {
		delete winSequences.takeLast();
	}
}

/* Called whenever a player marks a field.
 * Checks all possible lines to determin if current player won.
 */
void NoughtsAndCrosses::check() {
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

	auto isWin = [this] (WinCondition condition, WinSequence::Direction direction) {
		if (condition.first != -1) {
			auto sequence = new WinSequence(condition.first, direction);
			winSequences.append(sequence);
			emit winSequencesChanged();

			if (state != GameState::End) {
				setState(GameState::End);
				this->setWinner(condition);
			}
		}
	};

	// check vertical
	isWin(checkLines(1, 3), WinSequence::Vertical);

	// check horizontal
	isWin(checkLines(3, 1), WinSequence::Horizontal);

	// check diagonals
	isWin(checkLine(0, 4), WinSequence::LeftDiagonal);
	isWin(checkLine(2, 2), WinSequence::RightDiagonal);
}

QList<QObject*> NoughtsAndCrosses::getMap () const {
	return map;
}

void NoughtsAndCrosses::setState(NoughtsAndCrosses::GameState newState) {
	if (state != newState) {
		state = newState;
		emit stateChanged();
	}
}

void NoughtsAndCrosses::setWinner(WinCondition condition) {
	winner = (condition.second == Player::Nought ? player1 : player2);
	winner->wonGame();
	emit winnerChanged();
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

void NoughtsAndCrosses::startGame() {
	setState(GameState::Start);
}

void NoughtsAndCrosses::nextRound()
{
	for (auto i = 0; i < winSequences.size(); i++) {
		delete winSequences.takeLast();
	}
	emit winSequencesChanged();

	for (auto fieldObject : map) {
		auto field = static_cast<Field*>(fieldObject);
		field->setMark(Player::None);
	}

	setState(NoughtsAndCrosses::Start);
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

int NoughtsAndCrosses::getState() const {
	return state;
}

QObject *NoughtsAndCrosses::getWinner() const {
	return winner;
}

QList<QObject *> NoughtsAndCrosses::getWinSequences() const {
	return winSequences;
}

void NoughtsAndCrosses::changePlayer() {
	if (state == GameState::Playing) {
		currentPlayer = (player1 == currentPlayer) ? player2 : player1;
		emit currentPlayerChanged();
	}
}

int WinSequence::getFrom() const {
	return from;
}

int WinSequence::getDirection() const {
	return direction;
}

WinSequence::WinSequence(int setFrom, int setDirection) : from(setFrom), direction(setDirection) {
}
