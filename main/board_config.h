/**
 * @file board_config.h
 * @brief Waveshare ESP32-S3 Touch LCD 4.3" - Board Pin and Timing Definitions
 *
 * This file contains all hardware-specific pin assignments and LCD timing parameters
 * for the Waveshare ESP32-S3 Touch LCD 4.3" board (RGB 800x480 display).
 */

#pragma once

#include <stdint.h>

/**
 * @defgroup Board_Resolution
 * @{
 */
#define BOARD_WIDTH   800
#define BOARD_HEIGHT  480
/** @} */

/**
 * @defgroup I2C_Pins
 * @{
 */
#define I2C_HOST        0
#define I2C_SDA_GPIO    8
#define I2C_SCL_GPIO    9
#define I2C_FREQ_HZ     400000
/** @} */

/**
 * @defgroup Touch_GT911_Pins
 * @{
 */
#define GT911_I2C_ADDR  0x5D  // Default address, can be 0x14 if ADDR pin pulled high
#define GT911_IRQ_GPIO  4      // Touch interrupt
#define GT911_RST_EXIO  1      // Reset via CH422G EXIO1
/** @} */

/**
 * @defgroup Backlight_CH422G
 * @{
 */
#define CH422G_I2C_ADDR 0x20   // Default I2C address for CH422G
#define BL_ENABLE_EXIO  2      // Backlight enable via EXIO2 (active HIGH)
#define LCD_RST_EXIO    3      // LCD reset via EXIO3
/** @} */

/**
 * @defgroup LCD_RGB_Timings
 * Timing parameters calibrated for Waveshare 4.3" RGB panel
 * Source: Waveshare demo code (converted from PlatformIO config)
 * @{
 */
#define LCD_PCLK_HZ             (16 * 1000 * 1000)  // 16 MHz pixel clock
#define LCD_HSYNC_PULSE         4
#define LCD_HSYNC_BACK_PORCH    8
#define LCD_HSYNC_FRONT_PORCH   8
#define LCD_VSYNC_PULSE         4
#define LCD_VSYNC_BACK_PORCH    8
#define LCD_VSYNC_FRONT_PORCH   8
#define LCD_PCLK_ACTIVE_NEG     1  // 1: falling edge, 0: rising edge
/** @} */

/**
 * @defgroup LCD_RGB_DataPins (16-bit RGB565)
 * GPIO pin assignments for RGB565 data (R5G6B5)
 * Layout:
 *   [15:11] = R (GPIO40-42, 1-2)
 *   [10:5]  = G (GPIO21, 0, 45, 48, 47, 39)
 *   [4:0]   = B (GPIO10, 17, 18, 38, 14)
 * @{
 */
#define LCD_D0_GPIO   14   // B3 (LSB)
#define LCD_D1_GPIO   38   // B4
#define LCD_D2_GPIO   18   // B5
#define LCD_D3_GPIO   17   // B6
#define LCD_D4_GPIO   10   // B7
#define LCD_D5_GPIO   39   // G2
#define LCD_D6_GPIO   0    // G3
#define LCD_D7_GPIO   45   // G4
#define LCD_D8_GPIO   48   // G5
#define LCD_D9_GPIO   47   // G6
#define LCD_D10_GPIO  21   // G7
#define LCD_D11_GPIO  1    // R0
#define LCD_D12_GPIO  2    // R1
#define LCD_D13_GPIO  42   // R2
#define LCD_D14_GPIO  41   // R3
#define LCD_D15_GPIO  40   // R4 (MSB)
/** @} */

/**
 * @defgroup LCD_RGB_ControlPins
 * RGB panel control signals
 * @{
 */
#define LCD_HSYNC_GPIO  46
#define LCD_VSYNC_GPIO  3
#define LCD_DE_GPIO     5
#define LCD_PCLK_GPIO   7
/** @} */

/*
 * NOTE: This file uses `#pragma once` at the top. The trailing include
 * guard `#endif` was removed to avoid mismatch. If you prefer classic
 * include guards, replace `#pragma once` with proper `#ifndef`/`#define`/`#endif`.
 */
