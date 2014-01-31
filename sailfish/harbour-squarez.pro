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
    C++11\
    link_pkgconfig

PKGCONFIG += libcurl sailfishapp

INCLUDEPATH += ../src

INSTALLS += img

img.files = $$files(img/*.png)
img.path = /usr/share/$${TARGET}/img

SOURCES += src/squarez.cpp \
    ../src/shared/gameboard.cpp \
    ../src/client/highscores.cpp \
    ../src/client/httprequest.cpp \
    ../src/client/multiplayerrules.cpp \
    ../src/client/rules.cpp \
    ../src/client/singleplayerrules.cpp \
    ../src/client/tutorialrules.cpp \
    ../src/shared/score.cpp \
    ../src/shared/selection.cpp \
    ../src/shared/serializer.cpp \
    ../src/shared/timer.cpp \
    ../src/shared/transition.cpp \
    ../src/shared/network/methods.cpp \
    ../src/shared/network/urltools.cpp \
    ../src/qt/cell.cpp

HEADERS += \
    ../src/client/highscores.h \
    ../src/client/httprequest.h \
    ../src/client/multiplayerrules.h \
    ../src/client/rules.h \
    ../src/client/singleplayerrules.h \
    ../src/client/tutorialrules.h \
    ../src/shared/gameboard.h \
    ../src/shared/score.h \
    ../src/shared/selection.h \
    ../src/shared/serializer.h \
    ../src/shared/timer.h \
    ../src/shared/transition.h \
    ../src/shared/network/methods.h \
    ../src/shared/network/urltools.h \
    ../src/qt/cell.h

DEFINES += SQUAREZ_QT\
    PACKAGE=\\\"harbour-squarez\\\"\
    USERAGENT=\\\"jolla-squarez\\\"

OTHER_FILES += qml/squarez.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    rpm/squarez.spec \
    rpm/squarez.yaml \
    qml/squarez/GameArea.qml \
    qml/squarez/TimerArea.qml \
    qml/pages/SinglePlayerPage.qml \
    qml/pages/NameInput.qml \
    img/particle.png \
    harbour-squarez.desktop \
    qml/harbour-squarez.qml \
    qml/pages/TutorialPage.qml \
    rpm/harbour-squarez.yaml \
    qml/pages/MultiPlayerPage.qml
