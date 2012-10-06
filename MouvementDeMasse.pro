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

# libPropRoom2D
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Scaena-library/libPropRoom2D/release/ -lPropRoom2D
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Scaena-library/libPropRoom2D/debug/ -lPropRoom2D
else:unix:!macx:!symbian: LIBS += -L$$PWD/../Scaena-library/libPropRoom2D/ -lPropRoom2D
INCLUDEPATH += $$PWD/../Scaena-library/libPropRoom2D
DEPENDPATH += $$PWD/../Scaena-library/libPropRoom2D

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
    City/City.cpp \
    Algorithm/Algorithm.cpp \
    City/Junction.cpp \
    City/Land.cpp \
    Algorithm/HeightsAlgorithm/HeightsAlgorithm.cpp \
    Algorithm/HeightsAlgorithm/HeightsByNoiseAlgo.cpp \
    Algorithm/MapElementsAlgorithm/MapElementsAlgorithm.cpp \
    Algorithm/MapElementsAlgorithm/MapElementsDepthFirst.cpp \
    City/Street.cpp \
    City/Sun.cpp \
    Ui/MainWindow.cpp \
    City/Ground.cpp \
    Rendering/DrawCityModule.cpp \
    Rendering/SkyComponent.cpp \
    Rendering/WaterComponent.cpp \
    Rendering/GroundComponent.cpp \
    Rendering/JunctionsComponent.cpp \
    Rendering/StreetsComponent.cpp \
    MdMTypes.cpp \
    Algorithm/MapElementsAlgorithm/MapElementsByIsland.cpp \
    Rendering/BuildingsComponent.cpp \
    City/Sky.cpp \
    City/Citizen.cpp \
    Algorithm/CitizensAlgorithm/CitizensAlgorithm.cpp \
    Algorithm/CitizensAlgorithm/CitizensEqualAlgo.cpp \
    Rendering/BridgesComponent.cpp \
    City/Bridge.cpp \
    Rendering/CitizensComponent.cpp \
    City/Description.cpp \
    City/Island.cpp \
    Algorithm/CitizensAlgorithm/CitizensDistribByIsland.cpp \
    Algorithm/CitizensAlgorithm/CitizensMoveHomeWork.cpp

HEADERS += \
    MdMPlay.h \
    MdMCharacter.h \
    City/City.h \
    Algorithm/Algorithm.h \
    City/Junction.h \
    MdMTypes.h \
    City/Land.h \
    Algorithm/HeightsAlgorithm/HeightsAlgorithm.h \
    Algorithm/HeightsAlgorithm/HeightsByNoiseAlgo.h \
    Algorithm/MapElementsAlgorithm/MapElementsAlgorithm.h \
    Algorithm/MapElementsAlgorithm/MapElementsDepthFirst.h \
    City/Street.h \
    City/Sun.h \
    Ui/MainWindow.h \
    City/Ground.h \
    Rendering/DrawCityModule.h \
    Rendering/SkyComponent.h \
    Rendering/WaterComponent.h \
    Rendering/GroundComponent.h \
    Rendering/JunctionsComponent.h \
    Rendering/StreetsComponent.h \
    Algorithm/MapElementsAlgorithm/MapElementsByIsland.h \
    Rendering/BuildingsComponent.h \
    Rendering/AbstractComponent.h \
    City/Sky.h \
    City/Citizen.h \
    Algorithm/CitizensAlgorithm/CitizensAlgorithm.h \
    Algorithm/CitizensAlgorithm/CitizensEqualAlgo.h \
    Rendering/BridgesComponent.h \
    City/Bridge.h \
    Rendering/CitizensComponent.h \
    City/Description.h \
    City/Island.h \
    Algorithm/CitizensAlgorithm/CitizensDistribByIsland.h \
    Algorithm/CitizensAlgorithm/CitizensMoveHomeWork.h

OTHER_FILES += \
    resources/shaders/ground.vert \
    resources/shaders/ground.frag \
    resources/shaders/water.vert \
    resources/shaders/water.frag \
    resources/shaders/infrastruct.frag \
    resources/shaders/infrastruct.vert \
    resources/shaders/sky.vert \
    resources/shaders/sky.frag \
    resources/textures/street.bmp \
    resources/textures/roofTex.bmp \
    resources/textures/roofSpec.bmp \
    resources/textures/roof.xcf \
    resources/textures/junction.bmp \
    resources/textures/grass.bmp \
    resources/textures/commerceTex.bmp \
    resources/textures/commerceSpec.bmp \
    resources/textures/commerce.xcf \
    resources/textures/apartTex.bmp \
    resources/textures/apartSpec.bmp \
    resources/textures/apart.xcf \
    resources/fonts/Arial.flw \
    resources/fonts/Arial.bmp \
    resources/shaders/minimalist.frag \
    resources/shaders/minimalist.vert
