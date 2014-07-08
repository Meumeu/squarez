CONFIG += C++11\
    link_pkgconfig

QT += svg

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

img.files = \
    $$OUT_PWD/shape0.svg\
    $$OUT_PWD/shape1.svg\
    $$OUT_PWD/shape2.svg\
    $$OUT_PWD/shape0-selected.svg\
    $$OUT_PWD/shape1-selected.svg\
    $$OUT_PWD/shape2-selected.svg
img.path = /usr/share/$${TARGET}/img

SOURCES += \
    $$PWD/src/shared/board/gameboard.cpp \
    $$PWD/src/client/highscores.cpp \
    $$PWD/src/client/httprequest.cpp \
    $$PWD/src/client/multiplayerrules.cpp \
    $$PWD/src/shared/rules/rules.cpp \
    $$PWD/src/shared/rules/singleplayerrules.cpp \
    $$PWD/src/client/tutorialrules.cpp \
    $$PWD/src/shared/board/score.cpp \
    $$PWD/src/shared/board/selection.cpp \
    $$PWD/src/shared/serializer.cpp \
    $$PWD/src/shared/board/timer.cpp \
    $$PWD/src/shared/board/transition.cpp \
    $$PWD/src/shared/network/methods.cpp \
    $$PWD/src/shared/network/urltools.cpp \
    $$PWD/src/qt/cell.cpp

HEADERS += \
    $$PWD/src/client/highscores.h \
    $$PWD/src/client/httprequest.h \
    $$PWD/src/client/multiplayerrules.h \
    $$PWD/src/shared/rules/rules.h \
    $$PWD/src/shared/rules/singleplayerrules.h \
    $$PWD/src/client/tutorialrules.h \
    $$PWD/src/shared/board/gameboard.h \
    $$PWD/src/shared/board/score.h \
    $$PWD/src/shared/board/selection.h \
    $$PWD/src/shared/serializer.h \
    $$PWD/src/shared/board/timer.h \
    $$PWD/src/shared/board/transition.h \
    $$PWD/src/shared/network/methods.h \
    $$PWD/src/shared/network/urltools.h \
    $$PWD/src/qt/cell.h

DEFINES += SQUAREZ_QT
