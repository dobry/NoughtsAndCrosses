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

public:
	int getMark() const;
	int getScore() const;


	enum PlayerMark
	{
		None = 0, Nought, Cross
	};
	Q_ENUMS(PlayerMark)

	Player (PlayerMark mark);

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

	Field();

signals:
	void markChanged();
};


class NoughtsAndCrosses : public QObject {
	Q_OBJECT

	QList<QObject*> map;
	Q_PROPERTY(QList<QObject*> map READ getMap NOTIFY mapChanged)

	Player * player1;
	Q_PROPERTY(QObject* player1 READ getPlayer1 CONSTANT)

	Player * player2;
	Q_PROPERTY(QObject* player2 READ getPlayer2 CONSTANT)

	Player *currentPlayer;
	Q_PROPERTY(QObject* currentPlayer READ getCurrentPlayer NOTIFY currentPlayerChanged)

	int state;
	Q_PROPERTY(int state READ getState NOTIFY stateChanged)

	QList<QObject*> winSequences;
	Q_PROPERTY(QList<QObject*> winSequences READ getWinSequences NOTIFY winSequencesChanged)

	QObject *getCurrentPlayer () const;
	QObject *getPlayer1() const;
	QObject *getPlayer2() const;
	int getState() const;

	QList<QObject*> getWinSequences () const;

	void changePlayer();

public:
	enum GameState {
		Start, Playing, End
	};
	Q_ENUMS(GameState)

	Q_INVOKABLE void markField(const int fieldId);
	Q_INVOKABLE void startGame();

	QList<QObject*> getMap () const;
	void setState(NoughtsAndCrosses::GameState newState);

	explicit NoughtsAndCrosses(QObject *parent = 0);
	~NoughtsAndCrosses();


signals:
	void testChanged();
	void mapChanged();
	void currentPlayerChanged();
	void stateChanged();
	void winSequencesChanged();

public slots:
	void check();
};
Q_DECLARE_METATYPE(NoughtsAndCrosses::GameState)

#endif // NOUGHTSANDCROSSES_H
