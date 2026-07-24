add_library(menu STATIC
    LevelDesc.cpp LevelDesc.h
    LevelNode.cpp LevelNode.h
    NodeDrawer.cpp NodeDrawer.h
    PedoInput.cpp PedoInput.h
    Pedometer.cpp Pedometer.h
    WorldInput.cpp WorldInput.h
    WorldMap.cpp WorldMap.h
    WorldBranch.cpp WorldBranch.h
    worldmap-script.cpp worldmap-script.h
    SolverDrawer.cpp SolverDrawer.h
)
target_include_directories(menu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(menu PUBLIC gengine effect widget plan option state level)
