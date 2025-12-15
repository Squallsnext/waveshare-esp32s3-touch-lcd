/**
 * @file app_main.c
 * @brief Main application for Waveshare ESP32-S3 Touch LCD 4.3"
 *
 * Initializes:
 * - I2C bus and scans for devices
 * - Board configuration and hardware diagnostics
 *
 * NOTE: LVGL and LCD/Touch UI integration will be added in next phase
 * after managed components (lvgl, esp_lvgl_port) are installed.
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_heap_caps.h"

// Board-specific
#include "board_config.h"
#include "board_init.h"

static const char *TAG = "app_main";

/**
 * @brief Print system information at boot
 */
static void print_boot_info(void)
{
    ESP_LOGI(TAG, "=== Waveshare ESP32-S3 Touch LCD 4.3\" Boot ===");
    ESP_LOGI(TAG, "SoC: ESP32-S3 (Dual Core @ 240 MHz)");
    ESP_LOGI(TAG, "Display: 800x480 RGB LCD (16-bit RGB565)");
    ESP_LOGI(TAG, "Touch: GT911 (I2C GPIO8/9, IRQ GPIO4)");
    ESP_LOGI(TAG, "Backlight: CH422G IO Expander (EXIO2)");

    size_t free_heap = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
    size_t free_psram = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    ESP_LOGI(TAG, "Heap: %u bytes free", free_heap);
    ESP_LOGI(TAG, "PSRAM: %u bytes free", free_psram);
    ESP_LOGI(TAG, "");
}

/**
 * @brief Main application entry point
 *
 * Initialize hardware and prepare for LVGL UI via EEZ Studio export.
 */
void app_main(void)
{
    ESP_LOGI(TAG, "Starting application...\n");

    // Print boot diagnostics
    print_boot_info();

    // 1. Initialize I2C bus
    if (board_i2c_init() != ESP_OK) {
        ESP_LOGE(TAG, "I2C initialization failed!");
        return;
    }

    // 2. Scan I2C bus for devices (GT911 touch, CH422G IO expander)
    board_i2c_scan();

    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "=== Hardware initialized ===");
    ESP_LOGI(TAG, "Ready for LVGL UI integration.");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "Next steps:");
    ESP_LOGI(TAG, "1. Run: idf.py add-dependency \"lvgl/lvgl>=9.4\"");
    ESP_LOGI(TAG, "2. Run: idf.py add-dependency \"espressif/esp_lvgl_port>=2.0\"");
    ESP_LOGI(TAG, "3. Design UI in EEZ Studio (800x480, RGB565)");
    ESP_LOGI(TAG, "4. Export C code to main/ui/");
    ESP_LOGI(TAG, "5. Build: idf.py build");
    ESP_LOGI(TAG, "6. Flash: idf.py flash monitor");
    ESP_LOGI(TAG, "");

    // Idle loop
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
