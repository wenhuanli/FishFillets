add_library(option STATIC
    MenuOptions.cpp MenuOptions.h
    OptionsInput.cpp OptionsInput.h
    SelectLang.cpp SelectLang.h
    RadioBox.cpp RadioBox.h
    MenuHelp.cpp MenuHelp.h
    HelpInput.cpp HelpInput.h
)
target_include_directories(option PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(option PUBLIC gengine effect widget plan)
