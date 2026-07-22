cmake_minimum_required(VERSION 3.15)
project(Cobalt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)

add_executable(Cobalt
    src/main.cpp
)

target_include_directories(Cobalt PRIVATE include)

target_link_libraries(Cobalt PRIVATE raylib)

if(APPLE)
    target_link_libraries(Cobalt PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
elseif(UNIX AND NOT APPLE)
    target_link_libraries(Cobalt PRIVATE m pthread dl X11)
endif()
