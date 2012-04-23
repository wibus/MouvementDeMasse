
# Generals
QT      += opengl
QT      += gui
TARGET   = MouvementDeMasse
TEMPLATE = app
INCLUDEPATH += $$PWD/

# C++0x
win32:      QMAKE_CXXFLAGS += -std=gnu++0x
else:unix:  QMAKE_CXXFLAGS += -std=c++0x


# libCellarWorkBench
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Scaena-library/libCellarWorkbench/ -lCellarWorkbench
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Scaena-library/libCellarWorkbench/ -lCellarWorkbenchd
else:unix:!macx:!symbian: LIBS += -L$$PWD/../Scaena-library/libCellarWorkbench/ -lCellarWorkbench

INCLUDEPATH += $$PWD/../Scaena-library/libCellarWorkbench
DEPENDPATH += $$PWD/../Scaena-library/libCellarWorkbench


# Prop Room library
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Scaena-library/libPropRoom/ -lPropRoom
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Scaena-library/libPropRoom/ -lPropRoomd
else:unix:!macx:!symbian: LIBS += -L$$PWD/../Scaena-library/libPropRoom/ -lPropRoom

INCLUDEPATH += $$PWD/../Scaena-library/libPropRoom
DEPENDPATH += $$PWD/../Scaena-library/libPropRoom


# libScaena

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Scaena-library/libScaena/ -lScaena
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Scaena-library/libScaena/ -lScaenad
else:unix:!macx:!symbian: LIBS += -L$$PWD/../Scaena-library/libScaena/ -lScaena

INCLUDEPATH += $$PWD/../Scaena-library/libScaena
DEPENDPATH += $$PWD/../Scaena-library/libScaena

SOURCES += \
    main.cpp \
    MdMPlay.cpp \
    MdMCharacter.cpp

HEADERS += \
    MdMPlay.h \
    MdMCharacter.h
