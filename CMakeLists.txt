cmake_minimum_required(VERSION 3.10)
project(Cobalt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

add_executable(CXX src/main.cpp)

target_include_directories(MyGame PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(MyGame PRIVATE ${SDL2_LIBRARIES})