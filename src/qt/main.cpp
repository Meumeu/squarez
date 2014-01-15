#include <QtGui/QGuiApplication>
#include <QQmlComponent>
#include "qtquick2applicationviewer.h"

#include "client/singleplayerrules.h"
#include "client/highscores.h"
#include "shared/gameboard.h"
#include "qt/cell.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	qmlRegisterType<squarez::SinglePlayerRules>("Squarez", 1,0, "SinglePlayer");
	qmlRegisterType<squarez::HighScores>("Squarez", 1,0, "HighScores");
	qmlRegisterUncreatableType<squarez::GameBoard>("Squarez", 1,0, "GameBoard", "");
	qmlRegisterUncreatableType<squarez::qt::Cell>("Squarez", 1,0, "Cell", "");

	QtQuick2ApplicationViewer viewer;
	viewer.setMainQmlFile(QStringLiteral("qml/squarez/main.qml"));
	viewer.showExpanded();

	return app.exec();
}
