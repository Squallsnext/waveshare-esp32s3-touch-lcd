file(GLOB_RECURSE UI_SRCS "${CMAKE_CURRENT_LIST_DIR}/ui/*.c")

set(MAIN_SRCS
    app_main.c
    board_init.c
)

# Note: UI sources (ui/*.c) are collected above but not added to build
# until EEZ-generated sources are available and LVGL components are pinned.
# For now, only the core board code is built.
set(ALL_SRCS ${MAIN_SRCS})

idf_component_register(
    SRCS ${ALL_SRCS}
    INCLUDE_DIRS "." "ui"
    REQUIRES
        driver
        esp_timer
        freertos
        log
        eez_ui
)