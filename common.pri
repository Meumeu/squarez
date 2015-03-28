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
    $$PWD/src/game/gameboard.cpp \
    $$PWD/src/game/highscores.cpp \
    $$PWD/src/rules/rules.cpp \
    $$PWD/src/rules/singleplayerrules.cpp \
    $$PWD/src/game/score.cpp \
    $$PWD/src/game/selection.cpp \
    $$PWD/src/utils/serializer.cpp \
    $$PWD/src/rules/timer.cpp \
    $$PWD/src/game/transition.cpp \
    $$PWD/src/game/cell.cpp \
    $$PWD/src/proxy/qt/rulesproxy.cpp \
    $$PWD/src/proxy/qt/cellproxy.cpp

HEADERS += \
    $$PWD/src/game/gameboard.h \
    $$PWD/src/game/highscores.h \
    $$PWD/src/rules/rules.h \
    $$PWD/src/rules/singleplayerrules.h \
    $$PWD/src/game/score.h \
    $$PWD/src/game/selection.h \
    $$PWD/src/utils/serializer.h \
    $$PWD/src/rules/timer.h \
    $$PWD/src/game/transition.h \
    $$PWD/src/game/cell.h \
    $$PWD/src/proxy/qt/rulesproxy.h \
    $$PWD/src/proxy/qt/cellproxy.h

DEFINES += SQUAREZ_QT
