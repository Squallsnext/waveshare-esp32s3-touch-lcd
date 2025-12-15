/**
 * @file board_init.c
 * @brief Board initialization routines for Waveshare ESP32-S3 Touch LCD
 *
 * Handles I2C initialization, device scanning, and I/O expander setup.
 */

#include "board_init.h"
#include "board_config.h"
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "driver/gpio.h"
#include "esp_check.h"

static const char *TAG = "board_init";

// Global I2C bus handle
static i2c_master_bus_handle_t i2c_bus_handle = NULL;

/**
 * @brief Initialize I2C bus on configured pins
 *
 * @return ESP_OK on success, error code otherwise
 */
esp_err_t board_i2c_init(void)
{
    ESP_LOGI(TAG, "Initializing I2C on GPIO%d (SDA), GPIO%d (SCL) @ %d Hz",
             I2C_SDA_GPIO, I2C_SCL_GPIO, I2C_FREQ_HZ);

    i2c_master_bus_config_t i2c_bus_config = {
        .i2c_port = I2C_HOST,
        .sda_io_num = I2C_SDA_GPIO,
        .scl_io_num = I2C_SCL_GPIO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .intr_priority = 0,
        .trans_queue_depth = 0,  // Use default queue depth
        .flags = {
            .enable_internal_pullup = true,
        },
    };

    esp_err_t ret = i2c_new_master_bus(&i2c_bus_config, &i2c_bus_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create I2C bus: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "I2C bus initialized successfully");
    return ESP_OK;
}

/**
 * @brief Scan I2C bus and print found addresses
 *
 * Useful for verifying GT911 touch and CH422G presence.
 */
void board_i2c_scan(void)
{
    if (i2c_bus_handle == NULL) {
        ESP_LOGW(TAG, "I2C bus not initialized");
        return;
    }

    ESP_LOGI(TAG, "I2C Scan starting...");
    int devices_found = 0;

    for (uint8_t addr = 0x01; addr < 0x7F; addr++) {
        i2c_device_config_t dev_config = {
            .dev_addr_length = I2C_ADDR_BIT_LEN_7,
            .device_address = addr,
            .scl_speed_hz = I2C_FREQ_HZ,
        };

        i2c_master_dev_handle_t dev_handle;
        esp_err_t ret = i2c_master_bus_add_device(i2c_bus_handle, &dev_config, &dev_handle);
        if (ret != ESP_OK) {
            continue;  // Address doesn't respond
        }

        // Try a quick read to confirm device presence
        uint8_t dummy;
        ret = i2c_master_receive(dev_handle, &dummy, 1, pdMS_TO_TICKS(100));

        i2c_master_bus_rm_device(dev_handle);

        if (ret == ESP_OK) {
            devices_found++;
            const char *device_name = "Unknown";
            if (addr == 0x5D || addr == 0x14) {
                device_name = "GT911 (Touch)";
            } else if (addr == 0x20) {
                device_name = "CH422G (IO Expander)";
            }
            ESP_LOGI(TAG, "  Found device at 0x%02X (%s)", addr, device_name);
        }
    }

    if (devices_found == 0) {
        ESP_LOGW(TAG, "No I2C devices found. Check wiring and pull-ups.");
    } else {
        ESP_LOGI(TAG, "I2C Scan complete: %d device(s) found", devices_found);
    }
}

/**
 * @brief Get I2C bus handle
 *
 * @return Handle to I2C master bus, or NULL if not initialized
 */
i2c_master_bus_handle_t board_get_i2c_bus(void)
{
    return i2c_bus_handle;
}

/**
 * @brief Add I2C device to bus
 *
 * @param device_addr I2C address of device
 * @param dev_handle Output handle for added device
 *
 * @return ESP_OK on success, error code otherwise
 */
esp_err_t board_add_i2c_device(uint8_t device_addr, i2c_master_dev_handle_t *dev_handle)
{
    if (i2c_bus_handle == NULL) {
        ESP_LOGE(TAG, "I2C bus not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = device_addr,
        .scl_speed_hz = I2C_FREQ_HZ,
    };

    return i2c_master_bus_add_device(i2c_bus_handle, &dev_config, dev_handle);
}

/**
 * @brief Remove I2C device from bus
 *
 * @param dev_handle Device handle returned by board_add_i2c_device()
 *
 * @return ESP_OK on success, error code otherwise
 */
esp_err_t board_remove_i2c_device(i2c_master_dev_handle_t dev_handle)
{
    return i2c_master_bus_rm_device(dev_handle);
}
