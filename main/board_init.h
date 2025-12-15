/**
 * @file board_init.h
 * @brief Board initialization declarations
 */

#pragma once

#include "esp_err.h"
#include "driver/i2c_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize I2C bus with configured pins and frequency
 */
esp_err_t board_i2c_init(void);

/**
 * @brief Scan I2C bus and log found devices
 *
 * Prints addresses and device names to log.
 */
void board_i2c_scan(void);

/**
 * @brief Get handle to I2C bus
 *
 * @return I2C bus handle, or NULL if not initialized
 */
i2c_master_bus_handle_t board_get_i2c_bus(void);

/**
 * @brief Add device to I2C bus
 *
 * @param device_addr 7-bit I2C address
 * @param dev_handle Output pointer for device handle
 *
 * @return ESP_OK on success
 */
esp_err_t board_add_i2c_device(uint8_t device_addr, i2c_master_dev_handle_t *dev_handle);

/**
 * @brief Remove device from I2C bus
 *
 * @param dev_handle Device handle to remove
 *
 * @return ESP_OK on success
 */
esp_err_t board_remove_i2c_device(i2c_master_dev_handle_t dev_handle);

#ifdef __cplusplus
}
#endif
