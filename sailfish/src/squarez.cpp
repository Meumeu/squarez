/*
 * Squarez puzzle game
 * Copyright (C) 2013-2015  Guillaume Meunier <guillaume.meunier@centraliens.net>
 * Copyright (C) 2013-2015  Patrick Nicolas <patricknicolas@laposte.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <qqml.h>
#include <sailfishapp.h>
#include <QTranslator>
#include <QLocale>
#include <QGuiApplication>

#include "rulesproxy.h"
#include "cellproxy.h"
#include "highscores.h"
#include "settingvalue.h"

int main(int argc, char *argv[])
{
	qmlRegisterType<squarez::qt::RulesProxy>("harbour.squarez", 1,0, "Rules");
	qmlRegisterUncreatableType<squarez::qt::CellProxy>("harbour.squarez", 1,0, "Cell", "");
	qmlRegisterType<squarez::qt::HighScores>("harbour.squarez", 1,0, "HighScores");
	qmlRegisterType<squarez::qt::SettingValue>("harbour.squarez", 1, 0, "SettingValue");

	QGuiApplication * app = SailfishApp::application(argc, argv);

	QTranslator translator;
	translator.load(QLocale::system().name(), SailfishApp::pathTo(QString("translations")).toLocalFile());

	app->installTranslator(&translator);

	return SailfishApp::main(argc, argv);
}
