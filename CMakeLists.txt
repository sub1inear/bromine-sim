cmake_minimum_required(VERSION 3.15)
project(bromine-sim LANGUAGES C CXX)

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

set(CMAKE_POLICY_VERSION_MINIMUM 3.10)
FetchContent_MakeAvailable(raylib)
unset(CMAKE_POLICY_VERSION_MINIMUM)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d
    GIT_TAG        v3.1.1
)

set(BOX2D_BUILD_TESTS OFF CACHE INTERNAL "")
set(BOX2D_BUILD_SAMPLES OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(box2d)

add_executable(${PROJECT_NAME}
    src/sim/cup.cpp
    src/sim/device.cpp
    src/sim/distance.cpp
    src/sim/field.cpp
    src/sim/imu.cpp
    src/sim/input.cpp
    src/sim/main.cpp
    src/sim/motor.cpp
    src/sim/pin.cpp
    src/sim/renderer.cpp
    src/sim/robot.cpp
    src/sim/screen.cpp
    src/sim/sim.cpp
    src/sim/state.cpp
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
    target_sources(${PROJECT_NAME} PRIVATE src/pros/rtos/MSVC-MinGW/port.c)
elseif(UNIX)
    set(PROS_PORT_NAME "Posix")
    target_sources(${PROJECT_NAME} PRIVATE
        src/pros/rtos/Posix/port.c
        src/pros/rtos/Posix/utils/wait_for_event.c
    )
else()
    message(FATAL_ERROR "No FreeRTOS port available for this platform")
endif()

message(STATUS "${PROJECT_NAME}: using the ${PROS_PORT_NAME} FreeRTOS port")

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE include/sim)
target_include_directories(${PROJECT_NAME} PRIVATE include/pros)
target_include_directories(${PROJECT_NAME} PRIVATE include/sdk)
target_include_directories(${PROJECT_NAME} PRIVATE include/pros/rtos)
target_include_directories(${PROJECT_NAME} PRIVATE include/pros/rtos/${PROS_PORT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE ${raylib_SOURCE_DIR}/src/external/glfw/include)
target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE box2d::box2d)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
elseif(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE m pthread dl X11)
endif()

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE winmm)
endif()

if(WIN32 AND NOT MINGW AND CMAKE_CXX_COMPILER_LINKER_ID STREQUAL "LLD")
    target_link_options(${PROJECT_NAME} PRIVATE "LINKER:-lldmingw")
    foreach(dir $ENV{LIB})
        target_link_options(${PROJECT_NAME} PRIVATE "LINKER:/libpath:${dir}")
    endforeach()
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
add_dependencies(${PROJECT_NAME} pros_build_info)
set_source_files_properties("${PROS_BUILD_INFO_C}" PROPERTIES GENERATED TRUE)
target_sources(${PROJECT_NAME} PRIVATE "${PROS_BUILD_INFO_C}")

target_compile_definitions(${PROJECT_NAME} PRIVATE
    BROMINE_SIM=1
    BROMINE_SIM_PROJECT_NAME="${PROJECT_NAME}"
)

set(PROS_PUBLIC_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/pros_public_include")

file(GLOB PROS_PUBLIC_HEADERS CONFIGURE_DEPENDS
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/include/pros"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/pros/api.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/pros/pros/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/pros/pros/*.hpp"
)
foreach(header ${PROS_PUBLIC_HEADERS})
    set(FORWARD_HEADER_TARGET "${CMAKE_CURRENT_SOURCE_DIR}/include/pros/${header}")
    configure_file(cmake/forward_header.h.in "${PROS_PUBLIC_INCLUDE_DIR}/${header}" @ONLY)
endforeach()

set(USER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/user")
set(USER_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/user/include")

file(GLOB_RECURSE USER_SOURCE_PATHS CONFIGURE_DEPENDS FOLLOW_SYMLINKS
    RELATIVE "${USER_SOURCE_DIR}"
    "${USER_SOURCE_DIR}/*.cpp"
    "${USER_SOURCE_DIR}/*.c"
)

set(USER_SOURCES "")
foreach(path ${USER_SOURCE_PATHS})
    if(path MATCHES "^src/(.*)$" OR path MATCHES "^[^/]+/src/(.*)$")
        get_filename_component(dir "${CMAKE_MATCH_1}" DIRECTORY)
    else()
        continue()
    endif()

    set(source "${USER_SOURCE_DIR}/${path}")
    set_source_files_properties("${source}" PROPERTIES
        COMPILE_OPTIONS "-iquote;${USER_INCLUDE_DIR}/${dir}"
    )
    list(APPEND USER_SOURCES "${source}")
endforeach()

add_library(${PROJECT_NAME}-user OBJECT ${USER_SOURCES})

target_include_directories(${PROJECT_NAME}-user PRIVATE
    "${PROS_PUBLIC_INCLUDE_DIR}"
    include/user/include
)

target_compile_definitions(${PROJECT_NAME}-user PRIVATE
    BROMINE_SIM=1
    FMT_CONSTEVAL= # fmt library portability hack
)

set(NEWLIB_COMPAT_H "${CMAKE_CURRENT_SOURCE_DIR}/include/compat/newlib_compat.h")
target_compile_options(${PROJECT_NAME}-user PRIVATE -include "${NEWLIB_COMPAT_H}")
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-user)

if(EXISTS "${USER_INCLUDE_DIR}/liblvgl/llemu.hpp")
    set(LIBLVGL_PATCH_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/cmake/patch_liblvgl.cmake")
    file(SHA256 "${LIBLVGL_PATCH_SCRIPT}" LIBLVGL_PATCH_HASH)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${LIBLVGL_PATCH_SCRIPT}")

    FetchContent_Declare(
        liblvgl
        GIT_REPOSITORY https://github.com/purduesigbots/liblvgl.git
        GIT_TAG        4b6f16e8baf592511ad7618547ade755858fc79b # update/9.2.0, no release tag
        PATCH_COMMAND  "${CMAKE_COMMAND}"
                       -DLIBLVGL_SOURCE_DIR=<SOURCE_DIR>
                       -DLIBLVGL_PATCH_HASH=${LIBLVGL_PATCH_HASH}
                       -P "${LIBLVGL_PATCH_SCRIPT}"
    )
    FetchContent_MakeAvailable(liblvgl)

    file(GLOB_RECURSE LIBLVGL_SOURCES CONFIGURE_DEPENDS
        "${liblvgl_SOURCE_DIR}/src/liblvgl/*.c"
        "${liblvgl_SOURCE_DIR}/src/liblvgl/*.cpp"
    )

    foreach(source ${LIBLVGL_SOURCES})
        file(RELATIVE_PATH path "${liblvgl_SOURCE_DIR}/src" "${source}")
        get_filename_component(dir "${path}" DIRECTORY)
        set_source_files_properties("${source}" PROPERTIES
            COMPILE_OPTIONS "-iquote;${liblvgl_SOURCE_DIR}/include/${dir}"
        )
    endforeach()

    if(WIN32)
        set_property(SOURCE "${liblvgl_SOURCE_DIR}/src/liblvgl/libs/fsdrv/lv_fs_stdio.c"
            APPEND PROPERTY COMPILE_DEFINITIONS WIN32)
    endif()

    add_library(${PROJECT_NAME}-lvgl OBJECT ${LIBLVGL_SOURCES})

    target_include_directories(${PROJECT_NAME}-lvgl PRIVATE
        "${PROS_PUBLIC_INCLUDE_DIR}"
        "${liblvgl_SOURCE_DIR}/include"
        "${liblvgl_SOURCE_DIR}/include/liblvgl"
    )

    target_compile_definitions(${PROJECT_NAME}-lvgl PRIVATE BROMINE_SIM=1)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-lvgl)

    target_compile_definitions(${PROJECT_NAME}-user PRIVATE
        _PROS_INCLUDE_LIBLVGL_LLEMU_H
        _PROS_INCLUDE_LIBLVGL_LLEMU_HPP
    )
endif()

# TODO: Have scripts/setup.py generate a config.cmake for library-specific shims.
