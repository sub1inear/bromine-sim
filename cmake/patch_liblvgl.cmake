# Patches liblvgl after FetchContent downloads it. Run in script mode:
#   cmake -DLIBLVGL_SOURCE_DIR=<dir> -P patch_liblvgl.cmake
#
# LVGL 9.2 stores enums in bitfields that are only wide enough for the enum's
# values as unsigned numbers (e.g. `lv_draw_sw_mask_line_side_t side : 2` for
# values 0-3). The MSVC ABI (used by Clang on Windows) gives enums a signed
# underlying type, so the upper values read back negative and comparisons such
# as `side == LV_DRAW_SW_MASK_LINE_SIDE_TOP` fail. This breaks, among other
# things, every diagonal line. GCC and the V5 toolchain treat these bitfields
# as unsigned, so the bug only shows up in the simulator on Windows.
#
# Upstream fixed this after 9.2.0 by widening the bitfields
# (https://github.com/lvgl/lvgl/pull/7171), but that changes struct layouts,
# and user code compiles against the PROS project's own unpatched copy of the
# LVGL headers. Instead, each field's type becomes `unsigned int` with the same
# width: the enums are int-sized, so storage units and bit positions are
# unchanged and only the library's reads stop sign-extending.
#
# Edits are plain text replacements rather than a .patch file so they work
# regardless of the checkout's line endings, and they are idempotent so the
# step can safely re-run.

if(NOT LIBLVGL_SOURCE_DIR)
    message(FATAL_ERROR "LIBLVGL_SOURCE_DIR must be set")
endif()

# file|enum type|field|bit width
set(BITFIELDS
    "include/liblvgl/draw/sw/lv_draw_sw_mask_private.h|lv_draw_sw_mask_line_side_t|side|2"
    "include/liblvgl/draw/lv_draw_line.h|lv_blend_mode_t|blend_mode|2"
    "include/liblvgl/draw/lv_draw_rect.h|lv_border_side_t|border_side|5"
    "include/liblvgl/draw/lv_draw_rect.h|lv_border_side_t|side|5"
    "include/liblvgl/misc/lv_style.h|lv_grad_dir_t|dir|3"
    "include/liblvgl/misc/lv_style.h|lv_grad_extend_t|extend|2"
    "include/liblvgl/widgets/label/lv_label_private.h|lv_label_long_mode_t|long_mode|3"
    "include/liblvgl/widgets/chart/lv_chart_private.h|lv_chart_update_mode_t|update_mode|1"
    "include/liblvgl/widgets/menu/lv_menu_private.h|lv_menu_mode_header_t|mode_header|2"
    "include/liblvgl/widgets/menu/lv_menu_private.h|lv_menu_mode_root_back_button_t|mode_root_back_btn|1"
)

foreach(entry ${BITFIELDS})
    string(REPLACE "|" ";" entry "${entry}")
    list(GET entry 0 file)
    list(GET entry 1 type)
    list(GET entry 2 field)
    list(GET entry 3 bits)

    set(path "${LIBLVGL_SOURCE_DIR}/${file}")
    file(READ "${path}" contents)

    set(original "${type}([ \t]+)${field}([ \t]*:[ \t]*${bits}[ \t]*;)")
    set(patched "unsigned int /\\* ${type} \\*/[ \t]+${field}[ \t]*:[ \t]*${bits}[ \t]*;")

    # Count without the trailing `;`, which would split each match in two
    # when MATCHALL builds its result list.
    string(REGEX MATCHALL "${type}[ \t]+${field}[ \t]*:[ \t]*${bits}[ \t]*" matches "${contents}")
    list(LENGTH matches match_count)
    if(match_count EQUAL 1)
        string(REGEX REPLACE "${original}" "unsigned int /* ${type} */\\1${field}\\2" contents "${contents}")
        file(WRITE "${path}" "${contents}")
        message(STATUS "liblvgl patch: ${file}: ${type} ${field} : ${bits} -> unsigned int")
    elseif(contents MATCHES "${patched}")
        message(STATUS "liblvgl patch: ${file}: ${field} already patched")
    else()
        message(FATAL_ERROR
            "liblvgl patch: expected exactly one `${type} ${field} : ${bits};` "
            "in ${file}, found ${match_count}. LVGL may have changed; "
            "update cmake/patch_liblvgl.cmake.")
    endif()
endforeach()
