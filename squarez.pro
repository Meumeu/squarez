# Add more folders to ship with the application, here
folder_01.source = qml/squarez
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

INCLUDEPATH += src

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += src/qt/main.cpp \
    src/shared/gameboard.cpp \
    src/client/highscores.cpp \
    src/client/httprequest.cpp \
#    src/client/multiplayerrules.cpp \
    src/client/rules.cpp \
    src/client/singleplayerrules.cpp \
#    src/client/tutorialrules.cpp \
    src/shared/score.cpp \
    src/shared/selection.cpp \
    src/shared/serializer.cpp \
    src/shared/timer.cpp \
    src/shared/transition.cpp \
    src/shared/network/methods.cpp \
    src/shared/network/urltools.cpp \
    src/qt/cell.cpp

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(src/qt/qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    src/client/highscores.h \
    src/client/httprequest.h \
    src/client/multiplayerrules.h \
    src/client/rules.h \
    src/client/singleplayerrules.h \
    src/client/tutorialrules.h \
    src/shared/gameboard.h \
    src/shared/score.h \
    src/shared/selection.h \
    src/shared/serializer.h \
    src/shared/timer.h \
    src/shared/transition.h \
    src/shared/network/methods.h \
    src/shared/network/urltools.h \
    src/qt/cell.h

CONFIG += c++11

DEFINES += SQUAREZ_QT
