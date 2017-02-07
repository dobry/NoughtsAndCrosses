#include "noughtsandcrosses.h"


int Player::getMark() const {
	return mark;
}

int Player::getScore() const {
	return score;
}

int Player::getIndex() const {
	return index;
}

void Player::wonGame() {
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

	auto isWin = [this] (WinCondition condition) {
		if (condition.first != -1) {
			if (state != GameState::End) {
				setState(GameState::End);
				this->setWinner(condition);
			}
		}
	};

	// check for winning sequences
	isWin(checkLines(1, 3)); // vertical
	isWin(checkLines(3, 1)); // horizontal
	isWin(checkLine(0, 4)); // left diagonal
	isWin(checkLine(2, 2)); // right diagonal

	// check if it's tie
	if (this->state != NoughtsAndCrosses::End) {
		// check if there is any empty field left
		for (auto i = 0; i < this->map.size(); i++) {
			if (getMark(i) == Player::None) {
				return;
			}
		}
		setState(NoughtsAndCrosses::Tie);
	}
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

void NoughtsAndCrosses::setCurrentPlayer(Player *player)
{
	if (currentPlayer != player) {
		currentPlayer = player;
		emit currentPlayerChanged();
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

void NoughtsAndCrosses::nextRound() {
	for (auto fieldObject : map) {
		auto field = static_cast<Field*>(fieldObject);
		field->setMark(Player::None);
	}

	winner = nullptr;
	setCurrentPlayer(player1->getScore() < player2->getScore() ? player1 : player2);

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

void NoughtsAndCrosses::changePlayer() {
	if (state == GameState::Playing) {
		setCurrentPlayer(player1 == currentPlayer ? player2 : player1);
	}
}
