
# Generals
QT          += opengl
TARGET       = MouvementDeMasse
TEMPLATE     = app
INCLUDEPATH += $$PWD/

# C++0x
win32:      QMAKE_CXXFLAGS += -std=gnu++0x
else:unix:  QMAKE_CXXFLAGS += -std=c++0x

# libCellarWorkBench
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Scaena-library/libCellarWorkbench/release/ -lCellarWorkbench
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Scaena-library/libCellarWorkbench/debug/ -lCellarWorkbench
else:unix: LIBS += -L$$OUT_PWD/../Scaena-library/libCellarWorkbench/ -lCellarWorkbench
INCLUDEPATH += $$PWD/../Scaena-library/libCellarWorkbench
DEPENDPATH += $$PWD/../Scaena-library/libCellarWorkbench

# Prop Room library
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Scaena-library/libPropRoom/release/ -lPropRoom
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Scaena-library/libPropRoom/debug/ -lPropRoom
else:unix: LIBS += -L$$OUT_PWD/../Scaena-library/libPropRoom/ -lPropRoom
INCLUDEPATH += $$PWD/../Scaena-library/libPropRoom
DEPENDPATH += $$PWD/../Scaena-library/libPropRoom

# libScaena
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Scaena-library/libScaena/release/ -lScaena
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Scaena-library/libScaena/debug/ -lScaena
else:unix: LIBS += -L$$OUT_PWD/../Scaena-library/libScaena/ -lScaena
INCLUDEPATH += $$PWD/../Scaena-library/libScaena
DEPENDPATH += $$PWD/../Scaena-library/libScaena

SOURCES += \
    main.cpp \
    MdMPlay.cpp \
    City.cpp \
    Intersection.cpp

HEADERS += \
    MdMPlay.h \
    City.h \
    Intersection.h
