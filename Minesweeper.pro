QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


CONFIG += c++17

CONFIG += static;

SOURCES += \
    main.cpp \
    minesweeper.cpp \
    qnewbutton.cpp

HEADERS += \
    minesweeper.h \
    qnewbutton.h

FORMS += \
    minesweeper.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    resources/Consolas/COPYRIGHT.txt \
    resources/Consolas/consolas.ttf \
    resources/bomb.png \
    resources/eight.png \
    resources/five.png \
    resources/flag.png \
    resources/four.png \
    resources/one.png \
    resources/seven.png \
    resources/six.png \
    resources/three.png \
    resources/two.png

RESOURCES += resources.qrc
