# VS Code Setup für Waveshare ESP32-S3 Touch LCD

## Schnellstart

### 1. Voraussetzungen
- **ESP-IDF 5.1+**: [Install Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/index.html#prerequisites)
- **VS Code**: [Download](https://code.visualstudio.com/)
- **ESP-IDF Extension**: Im VS Code Extension Marketplace suchen und installieren

### 2. ESP-IDF Environment laden
Vor dem Öffnen von VS Code:
```bash
. ~/esp/esp-idf/export.sh    # Adjust path to your ESP-IDF installation
code /Users/squallsnext/Documents/Programieren/waveshare-esp32s3-touch-lcd
```

Oder: In VS Code Terminal (`Ctrl+``) und `export.sh` sourcing (wird bei jedem neuen Terminal nötig):
```bash
. ~/esp/esp-idf/export.sh
```

### 3. Recommended VS Code Extensions
Das Projekt enthält `.vscode/extensions.json` mit Recommendations:
- **ESP-IDF Extension** (Espressif)
- **C/C++ Tools** (Microsoft)
- **CMake Tools** (Microsoft)
- **Makefile Tools** (Microsoft)
- **Cortex-Debug** (marus25) - optional für Hardware-Debug

### 4. Build & Flash im VS Code Terminal
```bash
# Build
idf.py build

# Flash (auto-detect COM port)
idf.py flash

# Monitor
idf.py monitor

# All in one
idf.py build flash monitor
```

### 5. VS Code Commands
Drücke `Ctrl+Shift+P` für Command Palette:
- `ESP-IDF: Build`
- `ESP-IDF: Flash`
- `ESP-IDF: Monitor`
- `ESP-IDF: Select Port`
- `ESP-IDF: Open ESP-IDF Terminal`

### 6. Benutzer-spezifische Einstellungen
Die `.vscode/settings.json` enthält:
- `idf.port`: Standard `/dev/cu.usbmodem*` (auto-detect) - ggf. anpassen
- `idf.flashBaudRate`: 921600 (optimal für ESP32-S3)
- `idf.monitorBaudRate`: 115200

Falls Probleme beim Port-Detect:
```bash
# Serial ports auflisten
ls -la /dev/cu.* /dev/tty.*
```

### 7. Debugging (optional)
Mit Cortex-Debug Extension und OpenOCD:
```bash
idf.py openocd
```

Dann in VS Code: Run & Debug (`Ctrl+Shift+D`) → "ESP-IDF: Debug"

## Troubleshooting

### "idf.py command not found"
- ESP-IDF Environment nicht geladen
- Lösung: `. ~/esp/esp-idf/export.sh` ausführen vor VS Code starten

### Build fails mit "Cannot find component"
- idf_component.yml Dependencies nicht geladen
- Lösung: `idf.py reconfigure` ausführen

### Monitor funktioniert nicht
- Port falsch konfiguriert
- Lösung: `ESP-IDF: Select Port` in Command Palette

### IntelliSense funktioniert nicht richtig
- C/C++ Tools Cache aktualisieren: `C/C++: Reset IntelliSense Database`
- Rebuild: `idf.py fullclean && idf.py build`

## Tipps

1. **Task-Shortcuts**: Nutze `.vscode/tasks.json` mit `Ctrl+Shift+B` (build shortcut)
2. **Terminal-Integration**: Alle idf.py Befehle laufen im VS Code Terminal
3. **Git Integration**: GitLens Extension zeigt Commit-History
4. **Formatting**: LLVM/Clang Format für C-Code automatisch bei Save

## Nächste Schritte

Nach erfolgreichem Build:
1. Hardware anstecken (USB-C)
2. `idf.py monitor` für Log-Output
3. Touch auf Display testen
4. SquareLine UI exportieren und integrieren (siehe main README)
