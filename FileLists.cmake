## HEADERS ##
# Algorithm #
# Citizen
SET(MOUVEMENT_DE_MASSE_ALGORITHM_CITIZEN_HEADERS
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/CitizensAlgorithm/CitizensAlgorithm.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/CitizensAlgorithm/CitizensDistribByIsland.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/CitizensAlgorithm/CitizensEqualAlgo.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/CitizensAlgorithm/CitizensMoveHomeWork.h)
    
# Height
SET(MOUVEMENT_DE_MASSE_ALGORITHM_HEIGHT_HEADERS
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/HeightsAlgorithm/HeightsAlgorithm.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/HeightsAlgorithm/HeightsByNoiseAlgo.h)
    
# Kruskal
SET(MOUVEMENT_DE_MASSE_ALGORITHM_KRUSKAL_HEADERS
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/Kruskal/KruskalAlgorithm.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/Kruskal/Segment.h)
    
# Map element
SET(MOUVEMENT_DE_MASSE_ALGORITHM_MAP_ELEMENT_HEADERS
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/MapElementsAlgorithm/MapElementsAlgorithm.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/MapElementsAlgorithm/MapElementsByIsland.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/MapElementsAlgorithm/MapElementsDepthFirst.h)
    
SET(MOUVEMENT_DE_MASSE_ALGORITHM_HEADERS
    ${MOUVEMENT_DE_MASSE_ALGORITHM_CITIZEN_HEADERS}
    ${MOUVEMENT_DE_MASSE_ALGORITHM_HEIGHT_HEADERS}
    ${MOUVEMENT_DE_MASSE_ALGORITHM_KRUSKAL_HEADERS}
    ${MOUVEMENT_DE_MASSE_ALGORITHM_MAP_ELEMENT_HEADERS}
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/Algorithm.h)
    
    
# City #
SET(MOUVEMENT_DE_MASSE_CITY_HEADERS
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/Bridge.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/Citizen.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/City.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/Description.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/Ground.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/Island.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/Junction.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/Land.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/Sky.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/Street.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/Sun.h)
    
    
# Rendering #
SET(MOUVEMENT_DE_MASSE_RENDERING_HEADERS
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Rendering/AbstractComponent.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Rendering/BridgesComponent.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Rendering/BuildingsComponent.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Rendering/CitizensComponent.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Rendering/DrawCityModule.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Rendering/GroundComponent.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Rendering/JunctionsComponent.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Rendering/SkyComponent.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Rendering/StreetsComponent.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Rendering/WaterComponent.h)


# UI #
SET(MOUVEMENT_DE_MASSE_UI_HEADERS
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Ui/MainWindow.h)

    
SET(MOUVEMENT_DE_MASSE_HEADERS
    ${MOUVEMENT_DE_MASSE_ALGORITHM_HEADERS}
    ${MOUVEMENT_DE_MASSE_CITY_HEADERS}
    ${MOUVEMENT_DE_MASSE_RENDERING_HEADERS}
    ${MOUVEMENT_DE_MASSE_UI_HEADERS}
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/MdMCharacter.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/MdMPlay.h
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/MdMTypes.h)
    
    
    
## SOURCES ##
# Algorithm #
# Citizen
SET(MOUVEMENT_DE_MASSE_ALGORITHM_CITIZEN_SOURCES
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/CitizensAlgorithm/CitizensAlgorithm.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/CitizensAlgorithm/CitizensDistribByIsland.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/CitizensAlgorithm/CitizensEqualAlgo.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/CitizensAlgorithm/CitizensMoveHomeWork.cpp)
    
# Height
SET(MOUVEMENT_DE_MASSE_ALGORITHM_HEIGHT_SOURCES
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/HeightsAlgorithm/HeightsAlgorithm.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/HeightsAlgorithm/HeightsByNoiseAlgo.cpp)
        
# Kruskal
SET(MOUVEMENT_DE_MASSE_ALGORITHM_KRUSKAL_SOURCES
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/Kruskal/KruskalAlgorithm.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/Kruskal/Segment.cpp)
    
# Map element
SET(MOUVEMENT_DE_MASSE_ALGORITHM_MAP_ELEMENT_SOURCES
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/MapElementsAlgorithm/MapElementsAlgorithm.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/MapElementsAlgorithm/MapElementsByIsland.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/MapElementsAlgorithm/MapElementsDepthFirst.cpp)
    
SET(MOUVEMENT_DE_MASSE_ALGORITHM_SOURCES
    ${MOUVEMENT_DE_MASSE_ALGORITHM_CITIZEN_SOURCES}
    ${MOUVEMENT_DE_MASSE_ALGORITHM_HEIGHT_SOURCES}
    ${MOUVEMENT_DE_MASSE_ALGORITHM_KRUSKAL_SOURCES}
    ${MOUVEMENT_DE_MASSE_ALGORITHM_MAP_ELEMENT_SOURCES}
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Algorithm/Algorithm.cpp)
        
# City #
SET(MOUVEMENT_DE_MASSE_CITY_SOURCES
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/Bridge.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/Citizen.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/City.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/Description.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/Ground.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/Island.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/Junction.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/Land.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/Sky.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/Street.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/City/Sun.cpp)
        
# Rendering #
SET(MOUVEMENT_DE_MASSE_RENDERING_SOURCES
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Rendering/BridgesComponent.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Rendering/BuildingsComponent.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Rendering/CitizensComponent.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Rendering/DrawCityModule.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Rendering/GroundComponent.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Rendering/JunctionsComponent.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Rendering/SkyComponent.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Rendering/StreetsComponent.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Rendering/WaterComponent.cpp)


# UI #
SET(MOUVEMENT_DE_MASSE_UI_SOURCES
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/Ui/MainWindow.cpp)

    
SET(MOUVEMENT_DE_MASSE_SOURCES
    ${MOUVEMENT_DE_MASSE_ALGORITHM_SOURCES}
    ${MOUVEMENT_DE_MASSE_CITY_SOURCES}
    ${MOUVEMENT_DE_MASSE_RENDERING_SOURCES}
    ${MOUVEMENT_DE_MASSE_UI_SOURCES}
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/main.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/MdMCharacter.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/MdMPlay.cpp
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/MdMTypes.cpp)
    
    
    
## GLobal ##
SET(MOUVEMENT_DE_MASSE_SRC_FILES
    ${MOUVEMENT_DE_MASSE_HEADERS}
    ${MOUVEMENT_DE_MASSE_SOURCES})
    
SET(MOUVEMENT_DE_MASSE_CONFIG_FILES
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/CMakeLists.txt
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/FileLists.cmake
    ${MOUVEMENT_DE_MASSE_SRC_DIR}/LibLists.cmake)
