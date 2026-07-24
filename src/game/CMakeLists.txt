set(FILLETS_SOURCES
    Application.cpp Application.h
    GameAgent.cpp GameAgent.h
    main.cpp
)

if(MINGW)
    find_program(WINDRES_EXECUTABLE NAMES windres i386-mingw32msvc-windres i586-mingw32msvc-windres)
    if(WINDRES_EXECUTABLE)
        set(FILLETS_ICO_OBJ ${CMAKE_CURRENT_BINARY_DIR}/fillets_ico.o)
        add_custom_command(OUTPUT ${FILLETS_ICO_OBJ}
            COMMAND ${WINDRES_EXECUTABLE} fillets.rc -O coff -o ${FILLETS_ICO_OBJ}
            DEPENDS fillets.rc fillets.ico
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
        list(APPEND FILLETS_SOURCES ${FILLETS_ICO_OBJ})
    endif()
endif()

add_executable(fillets ${FILLETS_SOURCES})
target_link_libraries(fillets PRIVATE
    menu level state option plan widget effect gengine
)

install(TARGETS fillets RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
