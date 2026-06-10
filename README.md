# 📺 TV-B-Gone + IR Jammer
Portable IR Toolkit based on ESP32-C3 SuperMini

```
████████╗██╗   ██╗    ██████╗      ██████╗  ██████╗ ███╗   ██╗███████╗
╚══██╔══╝██║   ██║    ██╔══██╗    ██╔════╝ ██╔═══██╗████╗  ██║██╔════╝
   ██║   ██║   ██║    ██████╔╝    ██║  ███╗██║   ██║██╔██╗ ██║█████╗
   ██║   ╚██╗ ██╔╝    ██╔══██╗    ██║   ██║██║   ██║██║╚██╗██║██╔══╝
   ██║    ╚████╔╝     ██████╔╝    ╚██████╔╝╚██████╔╝██║ ╚████║███████╗
   ╚═╝     ╚═══╝      ╚═════╝      ╚═════╝  ╚═════╝ ╚═╝  ╚═══╝╚══════╝
```

Two modes in one device — IR Jammer and TV-B-Gone.
Turns off TVs and blocks IR remotes using a single ESP32-C3 SuperMini.

---

## ⚙️ How it works

### 📡 IR Jammer
Floods the IR channel with a continuous 38 kHz carrier signal.
Any TV within range will stop responding to its remote control for as long as the jammer is active.

### 📺 TV-B-Gone
Cycles through power-off IR codes for dozens of TV brands:
Samsung, LG, Sony, Panasonic, Philips, Sharp, Toshiba, Hitachi, Grundig, Vizio, Hisense, TCL and more.
Each code is sent and confirmed with an LED blink.

---

## 🔘 Controls

| Action | Result |
|---|---|
| Click **Boot** (GPIO9) | IR Jammer starts, runs forever |
| Click **Button** (GPIO3) while jamming | Jammer stops, TV-B-Gone starts |
| TV-B-Gone finishes | Returns to idle, waiting |
| **Reset** button | Stops everything immediately |

---

## 💡 LED Indicator

| State | LED (GPIO8) |
|---|---|
| Idle | ⚫ off |
| Jamming | 🔵 solid on |
| TV-B-Gone sending | 🔵 blinks per code |

---

## 🔌 Pinout

### 📡 IR Module
| Signal | GPIO |
|---|---|
| DATA | GPIO 10 |
| VCC | 5V (VBUS) |
| GND | GND |

### 🔘 Buttons
| Button | GPIO |
|---|---|
| Jammer (Boot) | GPIO 9 (built-in) |
| TV-B-Gone | GPIO 3 |


---

## 📦 Hardware

- ESP32-C3 SuperMini
- IR LED module (940nm) — works with any NPN transistor driver
- IR LEDs — more LEDs = more range (tested with 9 LEDs)
- Push button
- Battery or USB power

---

## ⚠️ Power Notes

The IR module should be powered from **5V (VBUS)**, not 3.3V.

- Using 3.3V will reduce range by ~50%
- More IR LEDs in parallel = wider beam angle and better coverage
- A transistor driver is required when using multiple LEDs

### Range Estimates

| IR LEDs | Range |
|---|---|
| 1 | ~3–5 m |
| 3 | ~7–10 m |
| 9 | ~12–15 m |

---

## 🛠 Flashing

**Arduino IDE 2.x**

Add to Preferences → Additional boards URL:
```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

1. Boards Manager → install **esp32 by Espressif**
2. Select board: **ESP32C3 Dev Module**
3. Place `tv_b_gone.ino` and `codes.h` in the same folder
4. Select port and flash

---

## 📝 Adding Custom Codes

IR codes are stored in `codes.h` in PROGMEM to save RAM.

```cpp
static const uint16_t PROGMEM myTimings[] = { /* durations in µs */ };
static const uint8_t  PROGMEM myPairs[]   = { /* mark/space index pairs */ };

// Add to allCodes[] array:
{ myTimings, myPairs, sizeof(myPairs), nRepeat, gapMicros },
```

IR code database: [IRDB on GitHub](https://github.com/probonopd/irdb)

---

*Based on the original TV-B-Gone by Mitch Altman / Limor Fried (Lady Ada)*
