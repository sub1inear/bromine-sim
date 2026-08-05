cmake_minimum_required(VERSION 3.15)
project(CobaltSim LANGUAGES CXX)

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


FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(json)

add_executable(CobaltSim
    src/cup.cpp
    src/field.cpp
    src/main.cpp
    src/motor.cpp
    src/pin.cpp
    src/renderer.cpp
    src/robot.cpp
    src/sim.cpp
    src/window.cpp
    src/utils.cpp
)

target_include_directories(CobaltSim PRIVATE include)

target_link_libraries(CobaltSim PRIVATE raylib)
target_link_libraries(CobaltSim PRIVATE nlohmann_json::nlohmann_json)

if(APPLE)
    target_link_libraries(CobaltSim PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
elseif(UNIX AND NOT APPLE)
    target_link_libraries(CobaltSim PRIVATE m pthread dl X11)
endif()


add_custom_command(
    TARGET CobaltSim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:CobaltSim>/assets
)
