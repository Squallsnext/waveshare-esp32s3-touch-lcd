file(GLOB_RECURSE UI_SRCS "${CMAKE_CURRENT_LIST_DIR}/ui/*.c")

set(MAIN_SRCS
    app_main.c
    board_init.c
)

list(APPEND ALL_SRCS ${MAIN_SRCS} ${UI_SRCS})

idf_component_register(
    SRCS ${ALL_SRCS}
    INCLUDE_DIRS "." "ui"
    REQUIRES
        driver
        esp_timer
        freertos
        log
        lvgl
        esp_lvgl_port
        esp_lcd
        esp_lcd_rgb_panel
        esp_lcd_touch_gt911
)