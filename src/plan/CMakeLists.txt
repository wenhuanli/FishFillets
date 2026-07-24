add_library(plan STATIC
    Command.h
    CommandQueue.cpp CommandQueue.h
    ConsoleInput.cpp ConsoleInput.h
    FishDialog.cpp FishDialog.h
    GameState.cpp GameState.h
    KeyConsole.cpp KeyConsole.h
    KeyDesc.cpp KeyDesc.h
    Keymap.cpp Keymap.h
    Planner.cpp Planner.h
    ScriptCmd.cpp ScriptCmd.h
    StateInput.cpp StateInput.h
    StateManager.cpp StateManager.h
    SubTitleAgent.cpp SubTitleAgent.h
    Title.cpp Title.h
    dialog-script.cpp dialog-script.h
)
target_include_directories(plan PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(plan PUBLIC gengine effect)
