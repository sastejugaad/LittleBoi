# Fork Changes & New Features

This fork extends the original [Tiny Desktop Robot](https://www.instructables.com/Tiny-Desktop-Robot-With-ESP32-and-FreeRTOS/) with the following additions.

---

## PlatformIO Support

The project can now be built and uploaded using [PlatformIO](https://platformio.org/) instead of the Arduino IDE.

**Setup:**
1. Open the root folder in VS Code with the PlatformIO extension installed
2. Run `pio run --target upload` to build and flash

The `platformio.ini` at the project root handles all dependencies automatically.

---

## Mobile Web Controller

The ESP32 now runs as a **WiFi Access Point** and hosts a mobile-friendly web interface.

**How to use:**
1. Flash the firmware
2. Connect your phone or PC to the WiFi network `LittleBoi_Robot` (password: `huiweimaev`)
3. Open a browser and go to `http://192.168.4.1`

**Web Interface:**

![Mobile Web Controller](docs/webcontroller_interface.png)
> *Screenshot of the mobile web controller running in a browser*

**Features:**
- Directional control buttons (forward / backward / left / right) arranged as a cross
- Hold a button to keep moving — release to stop
- Speed slider (50–255)
- Optional log panel showing sent commands and responses

**Control Flow:**

```mermaid
flowchart LR
    subgraph Phone / PC
        A[Browser\nhttp://192.168.4.1] -->|hold button| B[sends HTTP POST /cmd\nevery 100ms]
        A -->|release button| C[sends HTTP POST /cmd\nstop]
    end

    subgraph ESP32 Access Point
        B --> D[AsyncWebServer\nCore 0]
        C --> D
        D --> E[PhrasedData\nshared variable]
        E --> F[loop\nCore 0\nMotor control]
        E --> G[dispTask\nCore 1\nOLED eyes]
    end

    subgraph Legacy
        H[UDP App\nPort 1234] --> D
    end
```

**Control still works via UDP** on port `1234` (original app remains compatible).

---

## Files Added

| File | Description |
|---|---|
| `mobile_html.h` | HTML/CSS/JS for the web controller, served from PROGMEM |
| `platformio.ini` | PlatformIO build configuration |
