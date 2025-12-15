# Waveshare ESP32-S3 Touch LCD 4.3" - ESP-IDF Project

A clean, production-ready ESP-IDF (CMake) project for the Waveshare ESP32-S3 Touch LCD 4.3" display featuring:
- **RGB 800x480 Display** with LVGL 9.4 graphics
- **GT911 Touch Controller** on I2C (GPIO8/9)
- **CH422G IO Expander** for backlight control (EXIO2) and touch reset (EXIO1)
- **Double-Buffered Display** in PSRAM (OPI 8MB)
- **USB CDC Serial** for logging and debugging

## Prerequisites

### Hardware
- Waveshare ESP32-S3 Touch LCD 4.3"
- USB-C cable for flashing and serial communication

### Software
1. **ESP-IDF 5.1+** installed and configured:
   ```bash
   cd ~/esp
   git clone --recursive https://github.com/espressif/esp-idf.git -b release/v5.3
   cd esp-idf
   ./install.sh
   ```

2. **VS Code** with official ESP-IDF extension (optional but recommended):
   - Install "ESP-IDF" extension by Espressif Systems

## Quick Start

### 1. Clone and Setup
```bash
git clone <this-repo>
cd waveshare-esp32s3-touch-lcd
```

### 2. Set ESP-IDF Environment (if not in shell profile)
```bash
. ~/esp/esp-idf/export.sh
```

### 3. Build, Flash, and Monitor

**Build:**
```bash
idf.py build
```

**Flash (auto-detect COM port):**
```bash
idf.py flash
```

**Monitor (USB CDC on /dev/ttyACM0 or /dev/cu.usbmodem*):
```bash
idf.py monitor
```

**All in one:**
```bash
idf.py build flash monitor
```

### Expected Boot Output
```
I (00) boot: ESP-IDF v5.3.1 on ESP32-S3
...
I (xxx) board_init: I2C Scanner on GPIO8/9 @ 400kHz
I (xxx) board_init: Found device at 0x5D (GT911 Touch)
I (xxx) board_init: Found device at 0x20 (CH422G IO Expander)
I (xxx) board_init: PSRAM OK - free heap: 4456KB
I (xxx) lcd_init: Initializing RGB LCD (800x480, 16-bit RGB565)
I (xxx) lcd_init: LCD init complete
I (xxx) touch_init: GT911 touch init complete
I (xxx) backlight_init: CH422G backlight enabled (EXIO2)
I (xxx) lvgl_init: LVGL initialized, display FPS: 60Hz
I (xxx) lvgl_ui: Demo UI ready - press button on LCD
```

## Configuration

### Display Timings (RGB Panel)
Located in `main/board_config.h`:
- **PCLK**: 16 MHz
- **H-Sync Pulse**: 4px, Back Porch: 8px, Front Porch: 8px
- **V-Sync Pulse**: 4px, Back Porch: 8px, Front Porch: 8px
- **PCLK Active Edge**: Falling (1)

These are calibrated for the Waveshare 4.3" panel and sourced from their demo code.

### Board Pins
- **Touch I2C**: SDA GPIO8, SCL GPIO9, IRQ GPIO4
- **Touch Reset**: CH422G EXIO1
- **Backlight Enable**: CH422G EXIO2 (active HIGH)
- **LCD Reset**: CH422G EXIO3
- **RGB Data Pins**: See `main/board_config.h` (16-bit RGB565)

### LVGL Configuration
Edit `main/lv_conf.h` to customize:
- Display resolution: 800x480
- Color depth: 16-bit RGB565
- Tick rate: 1000Hz
- Double buffering enabled for smooth animation

## Troubleshooting

### Backlight stays dark
1. **Check I2C scan output** - should find 0x20 (CH422G)
2. **Verify EXIO2 output** - add debug log in `board_init()` after `ch422g_set_level()`
3. **Check power to backlight** - measure GPIO voltage on expanded pins
4. **Board defect** - try another EXIO pin; if all fail, hardware issue

### Touch not responding
1. **Check I2C scan** - should find 0x5D or 0x14 (GT911)
2. **Verify reset sequence** - IRQ should be high after touch init
3. **Check touch IRQ waveform** - use oscilloscope on GPIO4
4. **I2C pullups** - board should have 4.7k pullups on SDA/SCL

### Display shows garbage / colors inverted
1. **Wrong PCLK phase** - try toggling `PCLK_ACTIVE_NEG` in `board_config.h`
2. **RGB pin assignment** - verify pin mapping matches board schematic
3. **Framebuffer not in PSRAM** - ensure SPIRAM config enabled in `sdkconfig.defaults`

### Monitor shows "Task watchdog" error
1. **LVGL task too slow** - increase `LVGL_TICK_PERIOD_MS` or reduce screen complexity
2. **I2C hangs** - add timeout to I2C reads (see `board_init.c`)
3. **Memory exhaustion** - check `idf.py monitor` for heap fragmentation

### Can't flash / "Waiting for COM port"
1. **USB CDC disabled** - ensure `CONFIG_ESP_CONSOLE_USB_CDC=y` in `sdkconfig.defaults`
2. **Wrong board** - select "ESP32-S3" in `idf.py menuconfig -> Serial flasher config`
3. **Bricked device** - hold BOOT button while power-on, then flash with `--chip esp32s3`

## Performance Notes

- **PSRAM OPI Mode**: 8MB available for framebuffers and assets
- **CPU Speed**: 240 MHz (balanced performance/power)
- **FreeRTOS Tick**: 1000 Hz (precise LVGL animation)
- **Display Refresh**: 60 Hz (16ms per frame)
- **Estimated FPS**: 55-60 FPS with simple UI (label + button)

## Adding New Features

### Adding a Custom Component
```bash
mkdir -p components/my_component
# Add CMakeLists.txt and sources
idf.py build  # Component auto-discovered
```

### Using LVGL Demos
Uncomment in `main/app_main.c`:
```c
// lv_demo_benchmark();
// lv_demo_music();
```

### Adjusting CPU/Display Frequency
Edit `sdkconfig.defaults`:
- CPU: `CONFIG_ESP32S3_DEFAULT_CPU_FREQ_240` → `80` or `160`
- PCLK: Modify `RGB_CLK_HZ` in `board_config.h`

## License

This project is provided as-is. Use, modify, and distribute freely.

## Resources

- [ESP-IDF Documentation](https://docs.espressif.com/projects/esp-idf/)
- [LVGL Documentation](https://docs.lvgl.io/)
- [Waveshare Wiki](https://www.waveshare.com/wiki/ESP32-S3-Touch-LCD-4.3)
- [ESP32-S3 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)

## Changelog

### v1.0.0 (2025-12-15)
- Initial ESP-IDF CMake project setup
- LVGL 9.4 integration with double buffering
- GT911 touch driver with I2C polling
- CH422G backlight and reset control
- Full PSRAM support (OPI 8MB)
- USB CDC serial debugging
- Comprehensive troubleshooting guide
