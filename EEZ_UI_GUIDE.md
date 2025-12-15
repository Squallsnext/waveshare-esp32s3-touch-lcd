# EEZ UI Integration Guide

This guide explains how to use **EEZ Studio** (EEZ UI) for designing and generating LVGL UI code for this ESP-IDF project.

## About EEZ UI

**EEZ Studio** is a free, open-source tool for designing embedded UIs with:
- Visual editor for LVGL widgets
- Real-time preview
- C code generation (LVGL compatible)
- Component library support
- Better workflow than manual code

- **Website**: https://www.envisioningengineering.com/
- **GitHub**: https://github.com/eez-open/studio
- **Documentation**: https://github.com/eez-open/studio/wiki

## Setup in EEZ Studio

### Project Creation
1. **New Project** → Select **Embedded** or **Desktop**
2. **Display Settings**:
   - Resolution: **800 × 480** (exactly)
   - Color Format: **RGB565** (16-bit)
3. **Framework**: Select **LVGL** (8.3+)
4. **Generate**: Set output to `ui/eez/`

### Project Structure
```
eez_project.eez (stored locally, not in repo - or keep in separate folder)
├── screens/
│   ├── screen1
│   ├── screen2
│   └── ...
├── styles/
├── fonts/
├── images/
└── (UI tree structure)
```

## Export from EEZ Studio

### Export Settings
1. **Build** → **Build the project**
2. **Output**: Select **LVGL C Export**
3. **Target**: ESP32 (LVGL 8.3+)
4. **Output Path**: `<this-project>/ui/eez/`

### Export Output Structure
```
ui/eez/
├── ui.h                 (Main UI header)
├── ui.c                 (UI init and all screens)
├── screens.h            (Screen definitions)
├── fonts/
│   ├── font_name.h
│   ├── font_name.c
│   └── ...
├── images/
│   ├── image_name.h
│   ├── image_name.c
│   └── ...
└── styles.h             (Style definitions)
```

## Integration into ESP-IDF Project

### 1. Copy UI Files
After export from EEZ Studio:
```bash
# EEZ generates files in its output folder
# Copy to this project:
cp -r <eez-project-output>/* ui/eez/

# Result:
ui/
└── eez/
    ├── ui.h
    ├── ui.c
    ├── screens.h
    ├── fonts/
    ├── images/
    └── styles.h
```

### 2. Update CMakeLists.txt

The `main/CMakeLists.txt` already includes `eez_ui` component.

Create a UI component at `components/eez_ui/CMakeLists.txt`:

```cmake
idf_component_register(
    SRCS
        "../../ui/eez/ui.c"
        "../../ui/eez/fonts/*.c"
        "../../ui/eez/images/*.c"
    INCLUDE_DIRS
        "../../ui/eez"
    REQUIRES
        lvgl
)
```

Or simpler: Keep generated files in `ui/eez/` and reference them directly in `main/CMakeLists.txt`:

```cmake
idf_component_register(
    SRCS
        "app_main.c"
        "board_init.c"
        "../ui/eez/ui.c"
        "../ui/eez/fonts/*.c"
        "../ui/eez/images/*.c"
    INCLUDE_DIRS
        "."
        "../ui/eez"
    REQUIRES
        eez_ui
        # ... other components
)
```

### 3. Integrate UI into app_main.c

In `main/app_main.c`:

```c
#include "ui.h"  // From EEZ export

void app_main(void) {
    // ... initialization code (I2C, LCD, Touch, LVGL) ...

    // Create LVGL UI (EEZ-generated)
    ui_init();

    // Main loop
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
```

### 4. Build
```bash
idf.py clean
idf.py build
idf.py flash monitor
```

## File Organization

### Recommended Directory Structure
```
waveshare-esp32s3-touch-lcd/
├── main/
│   ├── app_main.c              (Hardware init + main loop)
│   ├── board_init.c/.h         (I2C, touch, backlight)
│   ├── board_config.h          (Pins & timings)
│   └── CMakeLists.txt
├── components/
│   └── eez_ui/                 (EEZ UI component - optional)
│       ├── CMakeLists.txt
│       └── (or keep in ui/eez/)
├── ui/
│   └── eez/                    ← EEZ exports go here
│       ├── ui.h
│       ├── ui.c
│       ├── screens.h
│       ├── fonts/
│       ├── images/
│       └── styles.h
├── CMakeLists.txt              (Root)
├── idf_component.yml
├── sdkconfig.defaults
└── README.md
```

## Workflow

### Development Cycle
1. **Design in EEZ Studio**: Screens, widgets, styles, fonts
2. **Preview**: Real-time preview in EEZ
3. **Build/Export**: EEZ → Build → Generate LVGL C code
4. **Copy Files**: Export output → `ui/eez/`
5. **Build ESP32**: `idf.py build`
6. **Flash & Test**: `idf.py flash monitor`
7. **Debug Events**: Add event handlers in `main/app_main.c`

### Tips
- **Fonts**: Use EEZ's built-in font manager (TTF → C)
- **Images**: Pre-optimize PNG/JPG before importing
- **Styles**: Define reusable styles in EEZ, not inline
- **Events**: EEZ generates stub callbacks; implement in `app_main.c`
- **Versioning**: Commit generated `ui/eez/` files in Git
- **Designer File**: Store `.eez` project separately (design/ folder) or outside repo

## Example: Button Click

### In EEZ Studio
1. Create a screen with a button widget
2. Name it: `btn_start`
3. EEZ generates: `ui_btn_start` object reference

### In app_main.c
```c
#include "ui.h"

// Event callback (EEZ may generate a stub)
void on_btn_start_click(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        printf("Start button clicked!\n");
        // Your logic here
    }
}

void app_main(void) {
    // ... init ...
    ui_init();

    // Register event handler
    lv_obj_add_event_cb(ui_btn_start, on_btn_start_click, LV_EVENT_CLICKED, NULL);

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
```

## Troubleshooting

### "ui.h not found"
- Check `ui/eez/` folder exists with generated files
- Verify include paths in `CMakeLists.txt`

### Display shows garbage
- Verify RGB565 color format in EEZ AND sdkconfig.defaults
- Check LVGL version compatibility

### Touch events don't fire
- Ensure touch driver initialized (GT911) in `board_init.c`
- Verify LVGL input device registered
- Check event callback is attached with `lv_obj_add_event_cb()`

### Build fails: "undefined reference to ui_*"
- Verify EEZ-generated `.c` files are listed in CMakeLists.txt
- Check for typos in SRCS paths
- Run `idf.py fullclean` and rebuild

### EEZ Studio won't export
- Ensure LVGL version is compatible (8.3+)
- Check display resolution is exactly 800×480
- Verify color format is RGB565

## Advantages of EEZ UI

✅ Visual editor → faster UI development
✅ Component library → reusable widgets
✅ Real-time preview
✅ Auto-generates LVGL C code
✅ Open source & free
✅ Good documentation & active community
✅ Touch support out-of-the-box

## Next Steps

1. **Install EEZ Studio**: https://www.envisioningengineering.com/
2. **Create Project**: 800×480, RGB565, LVGL
3. **Design UI**: Add screens, buttons, labels
4. **Export**: Generate C code
5. **Integrate**: Copy to `ui/eez/`
6. **Build & Flash**: `idf.py build flash monitor`

---

**For more details**, see:
- [EEZ Studio GitHub Wiki](https://github.com/eez-open/studio/wiki)
- [LVGL Documentation](https://docs.lvgl.io/)
- [Main README](./README.md)
