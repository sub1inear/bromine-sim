# Regenerates pros_build_info.c.

string(TIMESTAMP PROS_COMPILE_TIMESTAMP "%Y-%m-%d %H:%M:%S")
string(TIMESTAMP PROS_COMPILE_TIMESTAMP_INT "%s" UTC)

if(NOT PROS_COMPILE_TIMESTAMP_INT MATCHES "^[0-9]+$")
    set(PROS_COMPILE_TIMESTAMP_INT 0)
endif()

string(LENGTH "${PROS_COMPILE_DIRECTORY}" _dir_len)
if(_dir_len GREATER 29)
    math(EXPR _dir_start "${_dir_len} - 28")
    string(SUBSTRING "${PROS_COMPILE_DIRECTORY}" ${_dir_start} -1 PROS_COMPILE_DIRECTORY)
    string(PREPEND PROS_COMPILE_DIRECTORY "~")
endif()

configure_file("${TEMPLATE}" "${OUTPUT}" @ONLY)
