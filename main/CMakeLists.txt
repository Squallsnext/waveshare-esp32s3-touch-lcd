file(GLOB_RECURSE UI_SOURCES "ui/*.c")

idf_component_register(
    SRCS
        "app_main.c"
        "board_init.c"
        ${UI_SOURCES}
    INCLUDE_DIRS
        "."
        "ui"
    REQUIRES
        esp_lvgl_port
        lvgl
        esp_lcd_touch_gt911
        esp32_io_expander
        esp_timer
        driver
        freertos
        log
)
