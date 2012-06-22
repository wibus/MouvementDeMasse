
# Generals
QT      += opengl
QT      += gui
TARGET   = MouvementDeMasse
TEMPLATE = app
INCLUDEPATH += $$PWD/

# C++0x
win32:      QMAKE_CXXFLAGS += -std=gnu++0x
else:unix:  QMAKE_CXXFLAGS += -std=c++0x

#Lib Paths
win32: INCLUDEPATH += %DEV_HOME%/glew-1.7.0/include/
win32: LIBS += %DEV_HOME%/glew-1.7.0/bin/glew32.dll
else:unix: LIBS += -L/usr/lib -lGLEW

# libCellarWorkBench
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Scaena-library/libCellarWorkbench/release/ -lCellarWorkbench
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Scaena-library/libCellarWorkbench/debug/ -lCellarWorkbench
else:unix:!macx:!symbian: LIBS += -L$$PWD/../Scaena-library/libCellarWorkbench/ -lCellarWorkbench

INCLUDEPATH += $$PWD/../Scaena-library/libCellarWorkbench
DEPENDPATH += $$PWD/../Scaena-library/libCellarWorkbench


# Prop Room library
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Scaena-library/libPropRoom/release/ -lPropRoom
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Scaena-library/libPropRoom/debug/ -lPropRoom
else:unix:!macx:!symbian: LIBS += -L$$PWD/../Scaena-library/libPropRoom/ -lPropRoom

INCLUDEPATH += $$PWD/../Scaena-library/libPropRoom
DEPENDPATH += $$PWD/../Scaena-library/libPropRoom


# libScaena

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Scaena-library/libScaena/release/ -lScaena
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Scaena-library/libScaena/debug/ -lScaena
else:unix:!macx:!symbian: LIBS += -L$$PWD/../Scaena-library/libScaena/ -lScaena

INCLUDEPATH += $$PWD/../Scaena-library/libScaena
DEPENDPATH += $$PWD/../Scaena-library/libScaena

SOURCES += \
    main.cpp \
    MdMPlay.cpp \
    MdMCharacter.cpp \
    City/CityMap.cpp \
    Algorithm/Algorithm.cpp \
    Road/Junction.cpp \
    Land/Land.cpp \
    Algorithm/HeightsAlgorithm/HeightsAlgorithm.cpp \
    Algorithm/HeightsAlgorithm/HeightsByNoiseAlgo.cpp \
    Algorithm/DrawAlgorithm/DrawAlgorithm.cpp \
    Algorithm/MapElementsAlgorithm/MapElementsAlgorithm.cpp \
    Algorithm/MapElementsAlgorithm/MapElementsDepthFirst.cpp \
    Road/Street.cpp \
    City/Sun.cpp \
    Ui/MainWindow.cpp \
    City/Ground.cpp \
    Algorithm/DrawAlgorithm/DrawHighPrecisionGround.cpp \
    Algorithm/DrawAlgorithm/DrawCityModule.cpp \
    Algorithm/DrawAlgorithm/SunComponent.cpp \
    Algorithm/DrawAlgorithm/SkyComponent.cpp \
    Algorithm/DrawAlgorithm/WaterComponent.cpp \
    Algorithm/DrawAlgorithm/GroundComponent.cpp \
    Algorithm/DrawAlgorithm/JunctionsComponent.cpp \
    Algorithm/DrawAlgorithm/StreetsComponent.cpp \
    MdMTypes.cpp \
    Algorithm/MapElementsAlgorithm/MapElementsByIsland.cpp \
    Algorithm/DrawAlgorithm/ResidentialComponent.cpp

HEADERS += \
    MdMPlay.h \
    MdMCharacter.h \
    City/CityMap.h \
    Algorithm/Algorithm.h \
    Road/Junction.h \
    MdMTypes.h \
    Land/Land.h \
    Algorithm/HeightsAlgorithm/HeightsAlgorithm.h \
    Algorithm/HeightsAlgorithm/HeightsByNoiseAlgo.h \
    Algorithm/DrawAlgorithm/DrawAlgorithm.h \
    Algorithm/MapElementsAlgorithm/MapElementsAlgorithm.h \
    Algorithm/MapElementsAlgorithm/MapElementsDepthFirst.h \
    Road/Street.h \
    City/Sun.h \
    Ui/MainWindow.h \
    City/Ground.h \
    Algorithm/DrawAlgorithm/DrawHighPrecisionGround.h \
    Algorithm/DrawAlgorithm/DrawCityModule.h \
    Algorithm/DrawAlgorithm/SunComponent.h \
    Algorithm/DrawAlgorithm/SkyComponent.h \
    Algorithm/DrawAlgorithm/WaterComponent.h \
    Algorithm/DrawAlgorithm/GroundComponent.h \
    Algorithm/DrawAlgorithm/JunctionsComponent.h \
    Algorithm/DrawAlgorithm/StreetsComponent.h \
    Algorithm/MapElementsAlgorithm/MapElementsByIsland.h \
    Algorithm/DrawAlgorithm/ResidentialComponent.h

OTHER_FILES += \
    resources/shaders/sun.vert \
    resources/shaders/sun.frag \
    resources/shaders/ground.vert \
    resources/shaders/ground.frag \
    resources/shaders/roads.vert \
    resources/shaders/roads.frag \
    resources/shaders/highPrecGround.vert \
    resources/shaders/highPrecGround.frag \
    resources/shaders/water.vert \
    resources/shaders/water.frag










