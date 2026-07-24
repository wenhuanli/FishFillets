add_library(state STATIC
    DemoInput.cpp DemoInput.h
    DemoMode.cpp DemoMode.h
    GameInput.cpp GameInput.h
    PosterState.cpp PosterState.h
    PosterScroller.cpp PosterScroller.h
    demo-script.cpp demo-script.h
)
target_include_directories(state PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(state PUBLIC gengine effect widget plan option)
