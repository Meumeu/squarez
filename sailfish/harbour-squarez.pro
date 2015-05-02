# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = harbour-squarez

CONFIG += sailfishapp \
	link_pkgconfig \
	C++11 \
	link_pkgconfig

QT += svg

SVG_IMAGES = \
	$$PWD/../data/img/shape0.m4 \
	$$PWD/../data/img/shape1.m4 \
	$$PWD/../data/img/shape2.m4

svg.output = ${QMAKE_FILE_BASE}.svg
svg.commands = m4 -Dm4_config_file=${QMAKE_FILE_IN} $$PWD/../data/img/shape.svg.m4  > ${QMAKE_FILE_BASE}.svg
svg.input = SVG_IMAGES
svg.CONFIG = no_link target_predeps

svg_selected.output = ${QMAKE_FILE_BASE}-selected.svg
svg_selected.commands = m4 -Dm4_config_file=${QMAKE_FILE_IN} -Dselected $$PWD/../data/img/shape.svg.m4  > ${QMAKE_FILE_BASE}-selected.svg
svg_selected.input = SVG_IMAGES
svg_selected.CONFIG = no_link target_predeps

QMAKE_EXTRA_COMPILERS += svg svg_selected

INSTALLS += img qml2

img.files = \
	$$OUT_PWD/shape0.svg \
	$$OUT_PWD/shape1.svg \
	$$OUT_PWD/shape2.svg \
	$$OUT_PWD/shape0-selected.svg \
	$$OUT_PWD/shape1-selected.svg \
	$$OUT_PWD/shape2-selected.svg \
	$$PWD/../data/img/particle.png \
	$$PWD/harbour-squarez.svg
img.path = /usr/share/$${TARGET}/img

qml2.files = \
	$$PWD/../data/qml/GameArea.qml \
	$$PWD/../data/qml/SingleSquare.qml
qml2.path = /usr/share/$${TARGET}/qml/squarez

INCLUDEPATH += $$PWD/../shared $$PWD/../clients $$PWD/../clients/qml

SOURCES += src/squarez.cpp \
	../shared/game/cell.cpp \
	../shared/game/constants.cpp \
	../shared/game/score.cpp \
	../shared/game/transition.cpp \
	../shared/game/gameboard.cpp \
	../shared/game/selection.cpp \
	../shared/network/urltools.cpp \
	../shared/network/methods.cpp \
	../shared/rules/rules.cpp \
	../shared/rules/timer.cpp \
	../shared/rules/singleplayerrules.cpp \
	../shared/utils/serializer.cpp \
	../clients/qml/cellproxy.cpp \
	../clients/qml/highscores.cpp \
	../clients/qml/httprequest.cpp \
	../clients/qml/rulesproxy.cpp \
	../clients/qml/selectionproxy.cpp \
	../clients/qml/settingvalue.cpp \
	../clients/tutorialrules.cpp
# \
#    ../shared/database/database.cpp

HEADERS += \
	../shared/game/cell.h \
	../shared/game/constants.h \
	../shared/game/gameboard.h \
	../shared/game/score.h \
	../shared/game/transition.h \
	../shared/game/selection.h \
	../shared/network/urltools.h \
	../shared/network/methods.h \
	../shared/rules/timer.h \
	../shared/rules/singleplayerrules.h \
	../shared/rules/rules.h \
	../shared/utils/serializer.h \
	../clients/qml/cellproxy.h \
	../clients/qml/highscores.h \
	../clients/qml/rulesproxy.h \
	../clients/qml/selectionproxy.h \
	../clients/qml/settingvalue.h \
	../clients/httprequest.h
# \
#    ../shared/database/database.h


PKGCONFIG += sailfishapp

DEFINES += PACKAGE=\\\"harbour-squarez\\\"\
	PACKAGE_VERSION=\\\"3.0\\\"\
	override=
# override keyword is supported in gcc >= 4.7

OTHER_FILES += \
	qml/cover/CoverPage.qml \
	qml/harbour-squarez.qml \
	qml/pages/FirstPage.qml \
	qml/pages/HighScoreItem.qml \
	qml/pages/NameInput.qml \
	qml/pages/MultiPlayerPage.qml \
	qml/pages/SettingsPage.qml \
	qml/pages/SinglePlayerPage.qml \
	qml/pages/SwipeHint.qml \
	qml/pages/TutorialPage.qml \
	qml/squarez.qml \
	qml/squarez/GameArea.qml \
	qml/squarez/SingleSquare.qml \
	qml/squarez/Settings.qml \
	qml/squarez/TimerArea.qml \
	rpm/harbour-squarez.yaml \
	rpm/squarez.spec \
	rpm/squarez.yaml \
	img/particle.png \
	harbour-squarez.desktop \
	../translations/en.ts \
	../translations/fr.ts

RESOURCES +=

TRANSLATIONS = \
	$$PWD/../translations/en.ts \
	$$PWD/../translations/fr.ts

translation.output = ${QMAKE_FILE_BASE}.qm
translation.commands = lrelease ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_BASE}.qm
translation.input = TRANSLATIONS
translation.CONFIG = no_link target_predeps
QMAKE_EXTRA_COMPILERS += translation

translation_out.files = \
	$$OUT_PWD/en.qm \
	$$OUT_PWD/fr.qm
translation_out.path = /usr/share/$${TARGET}/translations

INSTALLS += translation_out

lupdate_only{
SOURCES += \
	qml/cover/CoverPage.qml \
	qml/harbour-squarez.qml \
	qml/pages/*.qml \
	qml/squarez.qml \
	qml/squarez/*.qml
}
