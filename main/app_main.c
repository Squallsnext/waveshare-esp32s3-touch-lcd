/**
 * @file app_main.c
 * @brief Main application for Waveshare ESP32-S3 Touch LCD 4.3"
 *
 * Initializes:
 * - I2C bus and scans for devices
 * - LCD RGB panel with LVGL
 * - GT911 touch controller
 * - CH422G IO expander (backlight + touch reset)
 * - Simple demo UI with label, button, and FPS counter
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_heap_caps.h"
#include "esp_timer.h"
#include "driver/gpio.h"

// Board-specific
#include "board_config.h"
#include "board_init.h"

// LVGL
#include "lvgl.h"
#include "esp_lvgl_port.h"

// Touch
#include "esp_lcd_touch_gt911.h"

// IO Expander
#include "esp32_io_expander_ch422g.h"

// LCD
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_panel_ops.h"

static const char *TAG = "app_main";

// Global handles
static esp_lcd_panel_handle_t panel_handle = NULL;
static esp_lcd_touch_handle_t touch_handle = NULL;
static esp_io_expander_handle_t io_expander_handle = NULL;
static lv_obj_t *label_status = NULL;
static lv_obj_t *label_fps = NULL;
static uint32_t frame_count = 0;
static int64_t fps_start_time = 0;

/**
 * @brief Button click event handler
 */
static void btn_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED) {
        ESP_LOGI(TAG, "Button clicked!");
        lv_label_set_text(label_status, "Button Clicked!");

        // Reset after 2 seconds
        vTaskDelay(pdMS_TO_TICKS(2000));
        lv_label_set_text(label_status, "Ready");
    }
}

/**
 * @brief Create simple demo UI
 */
static void create_demo_ui(void)
{
    // Create main screen
    lv_obj_t *scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1a1a1a), 0);

    // Title label
    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "ESP32-S3 Touch LCD 4.3\"");
    lv_obj_set_pos(title, 0, 20);
    lv_obj_set_width(title, BOARD_WIDTH);
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);

    // Status label
    label_status = lv_label_create(scr);
    lv_label_set_text(label_status, "Ready");
    lv_obj_set_pos(label_status, 0, 120);
    lv_obj_set_width(label_status, BOARD_WIDTH);
    lv_obj_set_style_text_align(label_status, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(label_status, lv_color_hex(0x00FF00), 0);

    // Demo Button
    lv_obj_t *btn = lv_button_create(scr);
    lv_obj_set_pos(btn, 300, 200);
    lv_obj_set_size(btn, 200, 60);
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Press Me");
    lv_obj_center(btn_label);
    lv_obj_set_style_text_font(btn_label, &lv_font_montserrat_16, 0);

    // FPS counter
    label_fps = lv_label_create(scr);
    lv_label_set_text(label_fps, "FPS: 0");
    lv_obj_set_pos(label_fps, 20, BOARD_HEIGHT - 50);
    lv_obj_set_style_text_color(label_fps, lv_color_hex(0xFFFF00), 0);

    // Memory info
    lv_obj_t *label_mem = lv_label_create(scr);
    lv_label_set_text(label_mem, "Heap: 0 KB");
    lv_obj_set_pos(label_mem, BOARD_WIDTH - 150, BOARD_HEIGHT - 50);
    lv_obj_set_style_text_color(label_mem, lv_color_hex(0x00FFFF), 0);

    frame_count = 0;
    fps_start_time = esp_timer_get_time();
}

/**
 * @brief Update FPS and memory display
 */
static void update_stats(void)
{
    frame_count++;
    int64_t now = esp_timer_get_time();
    int64_t elapsed = now - fps_start_time;

    if (elapsed >= 1000000) {  // 1 second
        uint32_t fps = (frame_count * 1000000) / elapsed;
        char fps_str[32];
        snprintf(fps_str, sizeof(fps_str), "FPS: %lu", fps);
        lv_label_set_text(label_fps, fps_str);

        // Update memory
        size_t free_heap = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
        size_t free_psram = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
        char mem_str[64];
        snprintf(mem_str, sizeof(mem_str), "RAM: %uKB PSRAM: %uKB",
                 free_heap / 1024, free_psram / 1024);
        // Note: Would need to add this label creation

        frame_count = 0;
        fps_start_time = now;
    }
}

/**
 * @brief Initialize LCD RGB panel
 *
 * @return ESP_OK on success
 */
static esp_err_t lcd_init(void)
{
    ESP_LOGI(TAG, "Initializing LCD RGB panel (%d x %d, 16-bit RGB565)",
             BOARD_WIDTH, BOARD_HEIGHT);

    esp_lcd_rgb_panel_config_t panel_config = {
        .data_width = 16,
        .bits_per_pixel = 16,
        .de_gpio_num = LCD_DE_GPIO,
        .pclk_gpio_num = LCD_PCLK_GPIO,
        .vsync_gpio_num = LCD_VSYNC_GPIO,
        .hsync_gpio_num = LCD_HSYNC_GPIO,
        .disp_gpio_num = -1,
        .on_frame_trans_done = NULL,
        .user_ctx = NULL,
        .flags = {
            .disp_active_low = 0,
            .relax_on_idle = 1,
            .disable_idle_ps = 0,
            .fb_in_psram = 1,  // Framebuffer in PSRAM
            .double_fb = 1,     // Double buffering
            .no_rgb_timing = 0,
            .pclk_active_neg = LCD_PCLK_ACTIVE_NEG,
        },
        .hsync_pulse_width = LCD_HSYNC_PULSE,
        .hsync_back_porch = LCD_HSYNC_BACK_PORCH,
        .hsync_front_porch = LCD_HSYNC_FRONT_PORCH,
        .vsync_pulse_width = LCD_VSYNC_PULSE,
        .vsync_back_porch = LCD_VSYNC_BACK_PORCH,
        .vsync_front_porch = LCD_VSYNC_FRONT_PORCH,
        .pclk_hz = LCD_PCLK_HZ,
        .bounce_buffer_size_px = BOARD_WIDTH * 10,
    };

    // Assign data pins
    int data_pins[] = {
        LCD_D0_GPIO, LCD_D1_GPIO, LCD_D2_GPIO, LCD_D3_GPIO,
        LCD_D4_GPIO, LCD_D5_GPIO, LCD_D6_GPIO, LCD_D7_GPIO,
        LCD_D8_GPIO, LCD_D9_GPIO, LCD_D10_GPIO, LCD_D11_GPIO,
        LCD_D12_GPIO, LCD_D13_GPIO, LCD_D14_GPIO, LCD_D15_GPIO,
    };
    panel_config.data_gpio_nums = data_pins;

    ESP_RETURN_ON_ERROR(esp_lcd_new_rgb_panel(&panel_config, &panel_handle),
                        TAG, "Failed to create RGB panel");

    ESP_RETURN_ON_ERROR(esp_lcd_panel_reset(panel_handle),
                        TAG, "Failed to reset panel");

    ESP_RETURN_ON_ERROR(esp_lcd_panel_init(panel_handle),
                        TAG, "Failed to init panel");

    ESP_LOGI(TAG, "LCD RGB panel initialized successfully");
    return ESP_OK;
}

/**
 * @brief Initialize LVGL display driver
 *
 * @return ESP_OK on success
 */
static esp_err_t lvgl_init(void)
{
    ESP_LOGI(TAG, "Initializing LVGL 9.4");

    esp_lvgl_port_cfg_t lvgl_cfg = {
        .io_pumping = {
            .run_all_on_same_core = true,
        },
    };

    ESP_RETURN_ON_ERROR(esp_lvgl_port_init(&lvgl_cfg),
                        TAG, "Failed to initialize LVGL port");

    const esp_lcd_panel_io_handle_t io_handle = NULL;  // RGB panel doesn't use io_handle
    const esp_lcd_panel_io_rgb_handle_t rgb_io = NULL;

    esp_lvgl_port_add_disp_config_t disp_cfg = {
        .panel_handle = panel_handle,
        .buffer_size = BOARD_WIDTH * BOARD_HEIGHT,
        .double_buffer = true,
        .flags = {
            .buff_dma = true,
            .buff_spiram = true,
        },
    };

    lv_disp_t *disp = esp_lvgl_port_add_disp(&disp_cfg);
    if (!disp) {
        ESP_LOGE(TAG, "Failed to add LVGL display");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "LVGL display registered");
    return ESP_OK;
}

/**
 * @brief Initialize touch controller (GT911)
 *
 * @return ESP_OK on success
 */
static esp_err_t touch_init(void)
{
    ESP_LOGI(TAG, "Initializing GT911 touch controller");

    i2c_master_bus_handle_t i2c_bus = board_get_i2c_bus();
    if (!i2c_bus) {
        ESP_LOGE(TAG, "I2C bus not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    esp_lcd_touch_config_t touch_cfg = {
        .x_max = BOARD_WIDTH,
        .y_max = BOARD_HEIGHT,
        .rst_gpio_num = -1,           // Reset via CH422G
        .int_gpio_num = GT911_IRQ_GPIO,
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = false,
            .mirror_x = false,
            .mirror_y = false,
        },
    };

    ESP_RETURN_ON_ERROR(esp_lcd_touch_new_i2c_gt911(i2c_bus, &touch_cfg, &touch_handle),
                        TAG, "Failed to initialize touch");

    ESP_RETURN_ON_ERROR(esp_lvgl_port_add_touch(touch_handle),
                        TAG, "Failed to add touch to LVGL");

    ESP_LOGI(TAG, "GT911 touch initialized successfully");
    return ESP_OK;
}

/**
 * @brief Initialize IO Expander (CH422G) for backlight and touch reset
 *
 * @return ESP_OK on success
 */
static esp_err_t io_expander_init(void)
{
    ESP_LOGI(TAG, "Initializing CH422G IO Expander");

    i2c_master_bus_handle_t i2c_bus = board_get_i2c_bus();
    if (!i2c_bus) {
        ESP_LOGE(TAG, "I2C bus not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    esp_io_expander_config_t config = {
        .i2c_address = CH422G_I2C_ADDR,
    };

    ESP_RETURN_ON_ERROR(esp_io_expander_new_ch422g(i2c_bus, &config, &io_expander_handle),
                        TAG, "Failed to create CH422G expander");

    // Configure all outputs
    ESP_RETURN_ON_ERROR(esp_io_expander_set_dir(io_expander_handle, 0xFF, 0),
                        TAG, "Failed to set output direction");

    // Reset touch (EXIO1: pulse low then high)
    ESP_RETURN_ON_ERROR(esp_io_expander_set_level(io_expander_handle, GT911_RST_EXIO, 0),
                        TAG, "Failed to reset touch");
    vTaskDelay(pdMS_TO_TICKS(10));
    ESP_RETURN_ON_ERROR(esp_io_expander_set_level(io_expander_handle, GT911_RST_EXIO, 1),
                        TAG, "Failed to release touch reset");
    vTaskDelay(pdMS_TO_TICKS(100));

    // Enable backlight (EXIO2: set HIGH)
    ESP_RETURN_ON_ERROR(esp_io_expander_set_level(io_expander_handle, BL_ENABLE_EXIO, 1),
                        TAG, "Failed to enable backlight");

    ESP_LOGI(TAG, "CH422G IO Expander initialized (backlight ON)");
    return ESP_OK;
}

/**
 * @brief Print system information at boot
 */
static void print_boot_info(void)
{
    ESP_LOGI(TAG, "=== Boot Information ===");
    ESP_LOGI(TAG, "ESP32-S3 SoC (Dual Core @ 240 MHz)");
    ESP_LOGI(TAG, "Display: 800x480 RGB LCD (16-bit RGB565)");
    ESP_LOGI(TAG, "PSRAM: 8MB OPI mode");

    size_t free_heap = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
    size_t free_psram = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    ESP_LOGI(TAG, "Heap: %u bytes free", free_heap);
    ESP_LOGI(TAG, "PSRAM: %u bytes free", free_psram);
}

/**
 * @brief Main application entry point
 */
void app_main(void)
{
    ESP_LOGI(TAG, "Starting Waveshare ESP32-S3 Touch LCD application...\n");

    // Print boot info
    print_boot_info();

    // 1. Initialize I2C bus
    if (board_i2c_init() != ESP_OK) {
        ESP_LOGE(TAG, "I2C initialization failed");
        return;
    }

    // 2. Scan for devices on I2C bus
    board_i2c_scan();

    // 3. Initialize IO Expander (CH422G) - for backlight and touch reset
    if (io_expander_init() != ESP_OK) {
        ESP_LOGE(TAG, "IO Expander initialization failed");
        return;
    }

    // 4. Initialize LCD RGB panel
    if (lcd_init() != ESP_OK) {
        ESP_LOGE(TAG, "LCD initialization failed");
        return;
    }

    // 5. Initialize LVGL
    if (lvgl_init() != ESP_OK) {
        ESP_LOGE(TAG, "LVGL initialization failed");
        return;
    }

    // 6. Initialize touch controller
    if (touch_init() != ESP_OK) {
        ESP_LOGE(TAG, "Touch initialization failed");
        return;
    }

    // 7. Create demo UI
    create_demo_ui();

    ESP_LOGI(TAG, "Application initialized successfully!");
    ESP_LOGI(TAG, "Display should show: Title, Status, Button, FPS Counter");

    // Main loop - update stats periodically
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10));
        update_stats();
    }
}
