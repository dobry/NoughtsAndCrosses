#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>

#include "noughtsandcrosses.h"
#include "iostream"

int main(int argc, char *argv[]) {
	QGuiApplication app(argc, argv);

	qmlRegisterType<NoughtsAndCrosses>("Game", 1, 0, "NoughtsAndCrosses");
	qmlRegisterUncreatableType<Player>("Game", 1, 0, "Player", "PlayerMark");

	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

	return app.exec();
}
