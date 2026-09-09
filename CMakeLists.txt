cmake_minimum_required(VERSION 3.15)
project(BromineSim LANGUAGES CXX)

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

include(FetchContent)

FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d
    GIT_TAG        v3.1.1
)

set(BOX2D_BUILD_TESTS OFF CACHE INTERNAL "")
set(BOX2D_BUILD_SAMPLES OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(box2d)

add_executable(BromineSim
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
    src/wheel.cpp
    src/device.cpp
    src/units.cpp
)

target_include_directories(BromineSim PRIVATE include)

target_link_libraries(BromineSim PRIVATE raylib)
target_link_libraries(BromineSim PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(BromineSim PRIVATE box2d::box2d)

if(APPLE)
    target_link_libraries(BromineSim PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
elseif(UNIX AND NOT APPLE)
    target_link_libraries(BromineSim PRIVATE m pthread dl X11)
endif()


add_custom_command(
    TARGET BromineSim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:BromineSim>/assets
)
