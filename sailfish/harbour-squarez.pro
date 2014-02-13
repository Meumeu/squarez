# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = harbour-squarez

include(../common.pri)

img.files += img/particle.png

CONFIG += sailfishapp \
    link_pkgconfig

PKGCONFIG += sailfishapp

SOURCES += src/squarez.cpp

HEADERS +=

DEFINES += \
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
    qml/pages/MultiPlayerPage.qml \
    qml/squarez/SingleSquare.qml \
    qml/squarez/Settings.qml
