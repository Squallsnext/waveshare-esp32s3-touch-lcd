# UI Directory - EEZ Studio

## Structure

```
ui/
└── eez/               ← EEZ Studio exports go here
    ├── ui.h
    ├── ui.c
    ├── screens.h
    ├── styles.h
    ├── fonts/
    │   ├── font_name.h
    │   └── font_name.c
    └── images/
        ├── image_name.h
        └── image_name.c
```

## Workflow

1. **Design UI** in [EEZ Studio](https://www.envisioningengineering.com/) (800×480, RGB565, LVGL 8.3+)
2. **Build/Export**: Build → Generate LVGL C code
3. **Copy Files**: `cp -r <eez-export>/* ui/eez/`
4. **Build ESP32**: `idf.py build`
5. **Flash & Test**: `idf.py flash monitor`

See [EEZ_UI_GUIDE.md](../EEZ_UI_GUIDE.md) for detailed instructions.

## Advantages of EEZ UI

✅ Free & open-source
✅ Visual editor for LVGL widgets
✅ Real-time preview
✅ Auto-generates C code
✅ Built-in font converter (TTF → C)
✅ Touch support out-of-the-box

## Notes

- Commit generated `ui/eez/` files so team/CI can build without EEZ
- Store EEZ `.eez` project file separately (design/ folder or outside repo)
- Images should be pre-optimized PNG/JPG
- Fonts: Use EEZ's font manager (automatic TTF → C conversion)
