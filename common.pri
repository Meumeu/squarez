CONFIG += C++11\
    link_pkgconfig

QMAKE_CXXFLAGS += -std=c++11

PKGCONFIG += libcurl

INCLUDEPATH += $$PWD/src

SVG_IMAGES = $$PWD/data/img/shape0.m4\
    $$PWD/data/img/shape1.m4\
    $$PWD/data/img/shape2.m4

svg.output = ${QMAKE_FILE_BASE}.svg
svg.commands = m4 -Dm4_config_file=${QMAKE_FILE_IN} $$PWD/data/img/shape.svg.m4  > ${QMAKE_FILE_BASE}.svg
svg.input = SVG_IMAGES
svg.CONFIG = no_link target_predeps

svg_selected.output = ${QMAKE_FILE_BASE}-selected.svg
svg_selected.commands = m4 -Dm4_config_file=${QMAKE_FILE_IN} -Dselected $$PWD/data/img/shape.svg.m4  > ${QMAKE_FILE_BASE}-selected.svg
svg_selected.input = SVG_IMAGES
svg_selected.CONFIG = no_link target_predeps

QMAKE_EXTRA_COMPILERS += svg svg_selected

INSTALLS += img

img.files = $$files(img/*.png)\
    $$OUT_PWD/shape0.svg\
    $$OUT_PWD/shape1.svg\
    $$OUT_PWD/shape2.svg\
    $$OUT_PWD/shape0-selected.svg\
    $$OUT_PWD/shape1-selected.svg\
    $$OUT_PWD/shape2-selected.svg
img.path = /usr/share/$${TARGET}/img

SOURCES += \
    $$PWD/src/shared/gameboard.cpp \
    $$PWD/src/client/highscores.cpp \
    $$PWD/src/client/httprequest.cpp \
    $$PWD/src/client/multiplayerrules.cpp \
    $$PWD/src/client/rules.cpp \
    $$PWD/src/client/singleplayerrules.cpp \
    $$PWD/src/client/tutorialrules.cpp \
    $$PWD/src/shared/score.cpp \
    $$PWD/src/shared/selection.cpp \
    $$PWD/src/shared/serializer.cpp \
    $$PWD/src/shared/timer.cpp \
    $$PWD/src/shared/transition.cpp \
    $$PWD/src/shared/network/methods.cpp \
    $$PWD/src/shared/network/urltools.cpp \
    $$PWD/src/qt/cell.cpp

HEADERS += \
    $$PWD/src/client/highscores.h \
    $$PWD/src/client/httprequest.h \
    $$PWD/src/client/multiplayerrules.h \
    $$PWD/src/client/rules.h \
    $$PWD/src/client/singleplayerrules.h \
    $$PWD/src/client/tutorialrules.h \
    $$PWD/src/shared/gameboard.h \
    $$PWD/src/shared/score.h \
    $$PWD/src/shared/selection.h \
    $$PWD/src/shared/serializer.h \
    $$PWD/src/shared/timer.h \
    $$PWD/src/shared/transition.h \
    $$PWD/src/shared/network/methods.h \
    $$PWD/src/shared/network/urltools.h \
    $$PWD/src/qt/cell.h

DEFINES += SQUAREZ_QT
