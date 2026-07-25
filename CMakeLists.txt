cmake_minimum_required(VERSION 3.16)
project(fillets VERSION 1.9 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)

###################################################
# SDL3 family - all ship native CMake config packages.
# Minimum versions are floors, not pins: SDL3 3.2.0 is required because
# the surface/pixel-format API this code uses (SDL_CreateSurface,
# SDL_GetPixelFormatForMasks, SDL_SetSurfaceRLE, ...) didn't exist before
# it; the *_mixer/*_ttf/*_image APIs used here have all been present
# since each library's first 3.0.0 release.
find_package(SDL3 3.2 REQUIRED CONFIG)
find_package(SDL3_mixer 3.0 REQUIRED CONFIG)
find_package(SDL3_image 3.0 REQUIRED CONFIG)
find_package(SDL3_ttf 3.0 REQUIRED CONFIG)

###################################################
# Lua and FriBidi via pkg-config
find_package(PkgConfig REQUIRED)
pkg_search_module(LUA REQUIRED IMPORTED_TARGET lua5.5 lua-5.5 "lua>=5.5")
pkg_check_modules(FRIBIDI IMPORTED_TARGET fribidi)

###################################################
# libm
if(UNIX)
    find_library(M_LIBRARY m)
endif()

###################################################
# Mung up the data dir the same way the old configure.in did: if nobody
# overrode datadir explicitly, install into share/games/<package> instead
# of plain share/.
if(CMAKE_INSTALL_DATADIR STREQUAL CMAKE_INSTALL_DATAROOTDIR)
    set(FILLETS_DATADIR "${CMAKE_INSTALL_DATADIR}/games/${PROJECT_NAME}")
else()
    set(FILLETS_DATADIR "${CMAKE_INSTALL_DATADIR}")
endif()

###################################################
# Debug on/off
option(ENABLE_DEBUG "Print debug messages" OFF)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        $<$<COMPILE_LANGUAGE:CXX>:-Wall>
        $<$<COMPILE_LANGUAGE:CXX>:-W>
        $<$<COMPILE_LANGUAGE:CXX>:-Wpointer-arith>
        $<$<COMPILE_LANGUAGE:CXX>:-Wcast-qual>
        $<$<COMPILE_LANGUAGE:CXX>:-Wcast-align>
        $<$<COMPILE_LANGUAGE:CXX>:-Woverloaded-virtual>
        $<$<COMPILE_LANGUAGE:CXX>:-Wconversion>
        $<$<COMPILE_LANGUAGE:CXX>:-Wshadow>
    )
    if(ENABLE_DEBUG)
        add_compile_options(
            $<$<COMPILE_LANGUAGE:CXX>:-g3>
            $<$<COMPILE_LANGUAGE:CXX>:-O0>
        )
    endif()
endif()

###################################################
# Shared dependency set - every module links this so SDL3/Lua/FriBidi
# include dirs and libs propagate transitively through the whole graph
# (mirrors the $(SDL_CFLAGS) $(LUA_CFLAGS) $(FRIBIDI_CFLAGS) that every
# Makefile.am pulled into its INCLUDES).
add_library(fillets_deps INTERFACE)
target_link_libraries(fillets_deps INTERFACE SDL3::SDL3 PkgConfig::LUA)
target_compile_definitions(fillets_deps INTERFACE LUA_COMPAT_APIINTCASTS)
if(FRIBIDI_FOUND)
    target_link_libraries(fillets_deps INTERFACE PkgConfig::FRIBIDI)
    target_compile_definitions(fillets_deps INTERFACE HAVE_FRIBIDI)
endif()
if(M_LIBRARY)
    target_link_libraries(fillets_deps INTERFACE ${M_LIBRARY})
endif()

add_subdirectory(src)

install(FILES fillets.6 DESTINATION ${CMAKE_INSTALL_MANDIR}/man6)

message(STATUS "")
message(STATUS "Fish Fillets are now configured (SDL3).")
message(STATUS "   FRIBIDI=${FRIBIDI_FOUND}")
message(STATUS "")
