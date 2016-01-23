TARGET = squarez

CONFIG += link_pkgconfig C++11

QMAKE_CXXFLAGS += -Wall -Wextra -Werror

QT += qml svg

INCLUDEPATH += $$PWD/shared $$PWD/clients $$PWD/clients/qml

SOURCES += clients/qml/main.cpp \
	shared/game/cell.cpp \
	shared/game/constants.cpp \
	shared/game/score.cpp \
	shared/game/transition.cpp \
	shared/game/gameboard.cpp \
	shared/game/selection.cpp \
	shared/network/urltools.cpp \
	shared/network/methods.cpp \
	shared/rules/rules.cpp \
	shared/rules/timer.cpp \
	shared/rules/singleplayerrules.cpp \
	shared/utils/serializer.cpp \
	clients/qml/cellproxy.cpp \
	clients/qml/highscores.cpp \
	clients/qml/httprequest.cpp \
	clients/qml/rulesproxy.cpp \
	clients/qml/selectionproxy.cpp \
	clients/qml/settingvalue.cpp \
	clients/tutorialrules.cpp

HEADERS += \
	shared/game/cell.h \
	shared/game/constants.h \
	shared/game/gameboard.h \
	shared/game/score.h \
	shared/game/transition.h \
	shared/game/selection.h \
	shared/network/urltools.h \
	shared/network/methods.h \
	shared/rules/timer.h \
	shared/rules/singleplayerrules.h \
	shared/rules/rules.h \
	shared/utils/serializer.h \
	clients/qml/cellproxy.h \
	clients/qml/highscores.h \
	clients/qml/rulesproxy.h \
	clients/qml/selectionproxy.h \
	clients/qml/settingvalue.h \
	clients/httprequest.h

DEFINES += PACKAGE=\\\"squarez\\\"\
	PACKAGE_VERSION=\\\"3.0\\\"

RESOURCES += data/resources.qrc

android {
    ANDROID_DEPLOYMENT_DEPENDENCIES = \
        jar/QtAndroid-bundled.jar \
        jar/QtAndroidAccessibility-bundled.jar \
        jar/QtMultimedia-bundled.jar \
        lib/libQt5Core.so \
        lib/libQt5Gui.so \
        lib/libQt5Network.so \
        lib/libQt5Qml.so \
        lib/libQt5Quick.so \
        lib/libQt5QuickParticles.so \
        lib/libQt5Multimedia.so \
        lib/libQt5MultimediaQuick_p.so \
        lib/libQt5Svg.so \
        lib/libQt5Widgets.so \
        plugins/imageformats/libqsvg.so \
        qml/QtQuick/Particles.2/qmldir \
        qml/QtQuick/Particles.2/libparticlesplugin.so \
        qml/QtQuick/Window.2/qmldir \
        qml/QtQuick/Window.2/libwindowplugin.so \
        qml/QtQuick/Layouts/qmldir \
        qml/QtQuick/Layouts/libqquicklayoutsplugin.so \
        qml/QtQuick/Controls/qmldir \
        qml/QtQuick/Controls/libqtquickcontrolsplugin.so \
        qml/QtQuick.2/qmldir \
        qml/QtQuick.2/libqtquick2plugin.so \
        qml/QtMultimedia/libdeclarative_multimedia.so \
        qml/QtMultimedia/qmldir

    exists($$[QT_INSTALL_PREFIX]/plugins/platforms/android/libqtforandroidGL.so) {
        # Qt < 5.3
        ANDROID_DEPLOYMENT_DEPENDENCIES += plugins/platforms/android/libqtforandroidGL.so
    } else {
        # Qt >= 5.3
        ANDROID_DEPLOYMENT_DEPENDENCIES += plugins/platforms/android/libqtforandroid.so
    }

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

android {
} else:unix {
OTHER_FILES += translations/en.ts \
	translations/fr.ts

TRANSLATIONS = \
	$$PWD/translations/en.ts \
	$$PWD/translations/fr.ts

translation_out.files = \
	$$OUT_PWD/en.qm \
	$$OUT_PWD/fr.qm
translation_out.path = /usr/share/$${TARGET}/translations

translation.output = ${QMAKE_FILE_BASE}.qm
translation.commands = lrelease ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_BASE}.qm
translation.input = TRANSLATIONS
translation.CONFIG = no_link target_predeps

QMAKE_EXTRA_COMPILERS += translation
INSTALLS += translation_out
}

lupdate_only {
SOURCES += \
	data/qml/*.qml \
	sailfish/qml/*.qml \
	sailfish/qml/cover/*.qml \
	sailfish/qml/pages/*.qml \
	sailfish/qml/squarez/*.qml
	
}
