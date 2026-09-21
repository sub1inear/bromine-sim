cmake_minimum_required(VERSION 3.15)
project(BromineSim LANGUAGES C CXX)

# Supress CMake warning that Raylib's CMake version is out of date.
cmake_policy(SET CMP0000 NEW)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

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
    src/sim/cup.cpp
    src/sim/device.cpp
    src/sim/field.cpp
    src/sim/main.cpp
    src/sim/motor.cpp
    src/sim/msg.cpp
    src/sim/pin.cpp
    src/sim/renderer.cpp
    src/sim/robot.cpp
    src/sim/sim.cpp
    src/sim/units.cpp
    src/sim/utils.cpp
    src/sim/wheel.cpp
    src/sim/window.cpp
    src/pros/common/cobs.c
    src/pros/common/gid.c
    src/pros/common/posix.c
    src/pros/common/linkedlist.c
    src/pros/common/set.c
    src/pros/common/string.c
    src/pros/devices/battery.c
    src/pros/devices/controller.c
    src/pros/devices/registry.c
    src/pros/devices/screen.c
    src/pros/devices/vdml.c
    src/pros/devices/vdml_adi.c
    src/pros/devices/vdml_ai_vision.c
    src/pros/devices/vdml_device.c
    src/pros/devices/vdml_distance.c
    src/pros/devices/vdml_ext_adi.c
    src/pros/devices/vdml_gps.c
    src/pros/devices/vdml_imu.c
    src/pros/devices/vdml_link.c
    src/pros/devices/vdml_motors.c
    src/pros/devices/vdml_optical.c
    src/pros/devices/vdml_rotation.c
    src/pros/devices/vdml_serial.c
    src/pros/devices/vdml_usd.c
    src/pros/devices/vdml_vision.c
    src/pros/devices/battery.cpp
    src/pros/devices/controller.cpp
    src/pros/devices/screen.cpp
    src/pros/devices/vdml_adi.cpp
    src/pros/devices/vdml_ai_vision.cpp
    src/pros/devices/vdml_device.cpp
    src/pros/devices/vdml_distance.cpp
    src/pros/devices/vdml_gps.cpp
    src/pros/devices/vdml_imu.cpp
    src/pros/devices/vdml_link.cpp
    src/pros/devices/vdml_motorgroup.cpp
    src/pros/devices/vdml_motors.cpp
    src/pros/devices/vdml_optical.cpp
    src/pros/devices/vdml_rotation.cpp
    src/pros/devices/vdml_serial.cpp
    src/pros/devices/vdml_usd.cpp
    src/pros/devices/vdml_vision.cpp
    src/pros/rtos/heap_4.c
    src/pros/rtos/list.c
    src/pros/rtos/queue.c
    src/pros/rtos/semphr.c
    src/pros/rtos/stream_buffer.c
    src/pros/rtos/tasks.c
    src/pros/rtos/task_notify_when_deleting.c
    src/pros/rtos/timers.c
    src/pros/rtos/rtos.cpp
    src/pros/system/envlock.c
    src/pros/system/hot.c
    src/pros/system/mlock.c
    src/pros/system/rtos_hooks.c
    src/pros/system/startup.c
    src/pros/system/system_daemon.c
    src/pros/system/unwind.c
    src/pros/system/user_functions.c
    src/pros/system/cpp_support.cpp
    src/pros/system/dev/dev_driver.c
    src/pros/system/dev/file_system_stubs.c
    src/pros/system/dev/ser_daemon.c
    src/pros/system/dev/ser_driver.c
    src/pros/system/dev/usd_driver.c
    src/pros/system/dev/vfs.c
    src/sdk/v5_api.cpp
    src/sdk/v5_apiuser.cpp
)

if(WIN32)
    set(PROS_PORT_NAME "MSVC-MinGW")
    target_sources(BromineSim PRIVATE src/pros/rtos/MSVC-MinGW/port.c)
elseif(UNIX)
    set(PROS_PORT_NAME "Posix")
    target_sources(BromineSim PRIVATE
        src/pros/rtos/Posix/port.c
        src/pros/rtos/Posix/utils/wait_for_event.c
    )
else()
    message(FATAL_ERROR "No FreeRTOS port available for this platform")
endif()

message(STATUS "BromineSim: using the ${PROS_PORT_NAME} FreeRTOS port")

target_include_directories(BromineSim PRIVATE include)
target_include_directories(BromineSim PRIVATE include/sim)
target_include_directories(BromineSim PRIVATE include/pros)
target_include_directories(BromineSim PRIVATE include/sdk)


target_include_directories(BromineSim PRIVATE include/pros/rtos)
target_include_directories(BromineSim PRIVATE include/pros/rtos/${PROS_PORT_NAME})

target_link_libraries(BromineSim PRIVATE raylib)
target_link_libraries(BromineSim PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(BromineSim PRIVATE box2d::box2d)

if(APPLE)
    target_link_libraries(BromineSim PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
elseif(UNIX AND NOT APPLE)
    target_link_libraries(BromineSim PRIVATE m pthread dl X11)
endif()

if(WIN32)
    target_link_libraries(BromineSim PRIVATE winmm)
endif()

set(PROS_BUILD_INFO_C "${CMAKE_CURRENT_BINARY_DIR}/pros_build_info.c")

set(PROS_BUILD_INFO_ARGS
    -DTEMPLATE=${CMAKE_CURRENT_SOURCE_DIR}/cmake/pros_build_info.c.in
    -DOUTPUT=${PROS_BUILD_INFO_C}
    -DPROS_COMPILE_DIRECTORY=${CMAKE_CURRENT_SOURCE_DIR}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/GenerateBuildInfo.cmake
)

execute_process(COMMAND ${CMAKE_COMMAND} ${PROS_BUILD_INFO_ARGS})

add_custom_target(pros_build_info
    COMMAND ${CMAKE_COMMAND} ${PROS_BUILD_INFO_ARGS}
    BYPRODUCTS "${PROS_BUILD_INFO_C}"
    COMMENT "Stamping PROS build info"
    VERBATIM
)
add_dependencies(BromineSim pros_build_info)
set_source_files_properties("${PROS_BUILD_INFO_C}" PROPERTIES GENERATED TRUE)
target_sources(BromineSim PRIVATE "${PROS_BUILD_INFO_C}")

file(GLOB USER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/user/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/user/*.c"
)
target_sources(BromineSim PRIVATE ${USER_SOURCES})

target_include_directories(BromineSim PRIVATE include/user)
