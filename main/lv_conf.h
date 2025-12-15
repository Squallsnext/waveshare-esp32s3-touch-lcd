/**
 * @file lv_conf.h
 * @brief LVGL 9.4 Configuration for Waveshare ESP32-S3 Touch LCD
 *
 * This is a minimal configuration optimized for the 800x480 RGB display.
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/**
 * @defgroup Display Configuration
 * @{
 */
#define LV_HOR_RES_MAX          (800)
#define LV_VER_RES_MAX          (480)
#define LV_COLOR_DEPTH          16   // 16-bit RGB565
#define LV_COLOR_16_SWAP        0
/** @} */

/**
 * @defgroup Memory and Buffers
 * @{
 */
#define LV_MEM_CUSTOM           0
#define LV_MEM_SIZE             (256 * 1024)

// Draw buffer - allocate in PSRAM by default via CONFIG_LV_USE_MALLOC_PSRAM
#define LV_DRAW_BUF_SIZE        (LV_HOR_RES_MAX * 100)
#define LV_DRAW_COMPLEX_BUF_SIZE (LV_HOR_RES_MAX * 50)

// Double buffering enabled
#define LV_DRAW_LAYER_SIMPLE    0
/** @} */

/**
 * @defgroup Tick and Task
 * @{
 */
#define LV_TICK_CUSTOM          1
#define LV_TICK_CUSTOM_INCLUDE  "esp_timer.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (esp_timer_get_time() / 1000)

#define LV_DISP_DEF_REFR_PERIOD 16  // 60 FPS
/** @} */

/**
 * @defgroup Feature Flags
 * @{
 */
#define LV_USE_LOG              1
#define LV_LOG_LEVEL            LV_LOG_LEVEL_INFO
#define LV_LOG_PRINTF           1

#define LV_USE_ASSERT_NULL      1
#define LV_USE_ASSERT_MALLOC    1
#define LV_USE_ASSERT_STYLE     1
#define LV_USE_ASSERT_MEM_INTEGRITY 1

#define LV_USE_PERF_MONITOR     1
#define LV_USE_MEM_MONITOR      1

#define LV_USE_SHADOW           1
#define LV_USE_BLEND_MODES      1
#define LV_USE_OUTLINE          1
#define LV_USE_OUTLINE_INVERT   1

/** @} */

/**
 * @defgroup Widget Support
 * @{
 */
#define LV_USE_LABEL            1
#define LV_USE_BUTTON           1
#define LV_USE_BUTTONMATRIX     1
#define LV_USE_CHECKBOX         1
#define LV_USE_DROPDOWN         1
#define LV_USE_IMAGE            1
#define LV_USE_SLIDER           1
#define LV_USE_ROLLER           1
#define LV_USE_SWITCH           1
#define LV_USE_TEXTAREA         1
#define LV_USE_TABLE            1
#define LV_USE_TABVIEW          1
#define LV_USE_SPINBOX          1
#define LV_USE_KEYBOARD         1
#define LV_USE_ARC              1
#define LV_USE_METER            1
#define LV_USE_GAUGE            1
#define LV_USE_MSGBOX           1
#define LV_USE_COLORWHEEL       1

/** @} */

/**
 * @defgroup Container Widgets
 * @{
 */
#define LV_USE_CONTAINER        1
#define LV_USE_OBJ              1
#define LV_USE_CANVAS           1
#define LV_USE_FLEX             1
#define LV_USE_GRID             1
/** @} */

/**
 * @defgroup Font Support
 * @{
 */
#define LV_FONT_MONTSERRAT_16   1
#define LV_FONT_MONTSERRAT_24   1
#define LV_FONT_MONTSERRAT_28   1
#define LV_FONT_DEFAULT         &lv_font_montserrat_16
/** @} */

/**
 * @defgroup Input Device Support
 * @{
 */
#define LV_USE_INDEV            1

// Touch support
#define LV_USE_TOUCH            1
#define LV_INDEV_DEF_READ_PERIOD 30  // 30ms polling
#define LV_INDEV_DEF_SCROLL_LIMIT 30

// Group for keyboard/button navigation
#define LV_USE_GROUP            1
#define LV_USE_OBJ_POINTER      1

/** @} */

/**
 * @defgroup OS Integration
 * @{
 */
#define LV_USE_OS               1
#define LV_OS_CUSTOM            1
#define LV_OS_CUSTOM_INCLUDE    "freertos/FreeRTOS.h"
/** @} */

/**
 * @defgroup Demos
 * @{
 */
#define LV_USE_DEMO_WIDGETS     0
#define LV_USE_DEMO_BENCHMARK   0
#define LV_USE_DEMO_MUSIC       0
#define LV_USE_DEMO_STRESS      0
#define LV_USE_DEMO_KEYPAD      0
/** @} */

#endif /* LV_CONF_H */
