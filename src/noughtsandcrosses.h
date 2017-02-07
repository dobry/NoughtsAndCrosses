#ifndef NOUGHTSANDCROSSES_H
#define NOUGHTSANDCROSSES_H

#include <QObject>
#include<array>

class Player : public QObject {
	Q_OBJECT

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


using WinCondition = std::pair<int, Player::PlayerMark>;


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


class NoughtsAndCrosses : public QObject {
	Q_OBJECT

	QList<QObject*> map;
	QList<QObject*> getMap () const;
	Q_PROPERTY(QList<QObject*> map READ getMap NOTIFY mapChanged)

	Player * player1;
	QObject *getPlayer1() const;
	Q_PROPERTY(QObject* player1 READ getPlayer1 CONSTANT)

	Player * player2;
	QObject *getPlayer2() const;
	Q_PROPERTY(QObject* player2 READ getPlayer2 CONSTANT)

	Player *currentPlayer;
	QObject *getCurrentPlayer () const;
	Q_PROPERTY(QObject* currentPlayer READ getCurrentPlayer NOTIFY currentPlayerChanged)

	Player *winner;
	QObject *getWinner() const;
	Q_PROPERTY(QObject* winner READ getWinner NOTIFY winnerChanged)

	int state;
	int getState() const;
	Q_PROPERTY(int state READ getState NOTIFY stateChanged)

	void changePlayer();

public:
	enum GameState {
		Start = 1, Playing, End, Tie
	};
	Q_ENUMS(GameState)

	Q_INVOKABLE void markField(const int fieldId);
	Q_INVOKABLE void startGame();
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
	void check();
};
Q_DECLARE_METATYPE(NoughtsAndCrosses::GameState)

#endif // NOUGHTSANDCROSSES_H
