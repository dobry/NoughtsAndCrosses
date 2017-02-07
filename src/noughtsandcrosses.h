#ifndef NOUGHTSANDCROSSES_H
#define NOUGHTSANDCROSSES_H

#include <QObject>


// class holds of a player
class Player : public QObject {
	Q_OBJECT

	// nought or cross
	int mark;
	Q_PROPERTY(int mark READ getMark CONSTANT)

	int score;
	Q_PROPERTY(int score READ getScore NOTIFY scoreChanged)

	int index;
	int getIndex() const;
	Q_PROPERTY(int index READ getIndex CONSTANT)

public:
	int getMark() const;
	int getScore() const;

	// called when the player wins, increments score
	void wonGame();

	enum PlayerMark
	{
		None = 0, Nought, Cross
	};
	Q_ENUMS(PlayerMark)

	explicit Player (PlayerMark mark, int setIndex);

signals:
	void scoreChanged(int score);
};
Q_DECLARE_METATYPE(Player::PlayerMark)


// type used to determin if there is a winning sequence
using WinCondition = std::pair<int, Player::PlayerMark>;


// holds state of one field on the map
class Field : public QObject {
	Q_OBJECT

	int mark;
	Q_PROPERTY(int mark READ getMark NOTIFY markChanged)

public:

	int getMark() const;
	void setMark(Player::PlayerMark mark);

	explicit Field();

signals:
	void markChanged();
};


// main game engine
class NoughtsAndCrosses : public QObject {
	Q_OBJECT

	// container with all game Fields
	QList<QObject*> map;
	QList<QObject*> getMap () const;
	Q_PROPERTY(QList<QObject*> map READ getMap NOTIFY mapChanged)

	Player *player1;
	QObject *getPlayer1() const;
	Q_PROPERTY(QObject* player1 READ getPlayer1 CONSTANT)

	Player *player2;
	QObject *getPlayer2() const;
	Q_PROPERTY(QObject* player2 READ getPlayer2 CONSTANT)

	// indicates which player's turn is it
	Player *currentPlayer;
	QObject *getCurrentPlayer () const;
	Q_PROPERTY(QObject* currentPlayer READ getCurrentPlayer NOTIFY currentPlayerChanged)

	// winner is only set when game state becomes GameState::End
	Player *winner;
	QObject *getWinner() const;
	Q_PROPERTY(QObject* winner READ getWinner NOTIFY winnerChanged)

	// main game state; `state`s changes determin game flow
	int state;
	int getState() const;
	Q_PROPERTY(int state READ getState NOTIFY stateChanged)

	void changePlayer();

public:
	enum GameState {
		Start = 1, Playing, End, Tie
	};
	Q_ENUMS(GameState)

	// marks Field under fieldId as belonging to currentPlayer and calls check on win conditions
	Q_INVOKABLE void markField(const int fieldId);

	// prepares game for new round, resets fields, etc.
	Q_INVOKABLE void nextRound();

	void setState(NoughtsAndCrosses::GameState newState);
	void setWinner(WinCondition condition);
	void setCurrentPlayer(Player* player);

	explicit NoughtsAndCrosses(QObject *parent = 0);
	~NoughtsAndCrosses();


signals:
	void testChanged();
	void mapChanged();
	void currentPlayerChanged();
	void stateChanged();
	void winnerChanged();

public slots:
	// checks whole map for winning sequences or tie, advances game accordingly
	void check();
};
Q_DECLARE_METATYPE(NoughtsAndCrosses::GameState)

#endif // NOUGHTSANDCROSSES_H
