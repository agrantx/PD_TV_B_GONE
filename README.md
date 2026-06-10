```text
████████╗██╗   ██╗    ██████╗      ██████╗  ██████╗ ███╗   ██╗███████╗
╚══██╔══╝██║   ██║    ██╔══██╗    ██╔════╝ ██╔═══██╗████╗  ██║██╔════╝
   ██║   ██║   ██║    ██████╔╝    ██║  ███╗██║   ██║██╔██╗ ██║█████╗
   ██║   ╚██╗ ██╔╝    ██╔══██╗    ██║   ██║██║   ██║██║╚██╗██║██╔══╝
   ██║    ╚████╔╝     ██████╔╝    ╚██████╔╝╚██████╔╝██║ ╚████║███████╗
   ╚═╝     ╚═══╝      ╚═════╝      ╚═════╝  ╚═════╝ ╚═╝  ╚═══╝╚══════╝
```
📺 TV-B-Gone + IR Jammer — Portable IR Toolkit based on ESP32-C3 SuperMini

---------------------------------------------------------------------------------
## 🌐 HTML Flasher 🌐 (beta)
[🌐 Open HTML Flasher](#)
⚠️ If the HTML Flasher does not work, use the manual flasher available [here](https://espressif.github.io/esptool-js/)
1. Open the flasher
2. Connect your ESP32-C3
3. Select the `tv_b_gone.ino.bin` file
4. Click **Flash** and wait for the process to complete

---------------------------------------------------------------------------------
## ⚙️ How it works

### 📡 IR Jammer
Floods the IR channel with a continuous 38 kHz carrier.
Any TV in range stops responding to its remote for as long as the jammer runs.

### 📺 TV-B-Gone
Cycles through power-off IR codes for dozens of TV brands:
Samsung, LG, Sony, Panasonic, Philips, Sharp, Toshiba, Hitachi, Grundig, Vizio, Hisense, TCL and more.
Each sent code is confirmed with an LED blink.

---------------------------------------------------------------------------------
## 🔘 Controls

| Action | Result |
|---|---|
| Click **Boot** (GPIO9) | IR Jammer starts, runs forever |
| Click **Button** (GPIO3) while jamming | Jammer stops → TV-B-Gone starts |
| TV-B-Gone finishes | Returns to idle |
| **Reset** button | Stops everything immediately |

## 💡 LED Indicator

| State | LED GPIO8 |
|---|---|
| Idle | ⚫ off |
| Jamming | 🔵 solid on |
| TV-B-Gone sending | 🔵 blinks per code |

---------------------------------------------------------------------------------
# 🔌 Pinout

### 📡 IR Module
| Signal | GPIO |
|---|---|
| DATA | GPIO 10 |
| VCC | 5V (VBUS) |
| GND | GND |

### 🔘 Buttons
| Button | GPIO |
|---|---|
| Jammer | GPIO 9 (Boot, built-in) |
| TV-B-Gone | GPIO 3 |

> ⚠️ IR module DATA must be on **GPIO10**, not GPIO9 — GPIO9 is reserved for the Boot button.

---------------------------------------------------------------------------------
# ⚠️ Power Notes
IR LEDs are powered from **5V (VBUS)**, not 3.3V.
* Using 3.3V reduces range by ~50%
* Use a transistor driver when running multiple LEDs in parallel
* More LEDs = wider beam and better coverage

---------------------------------------------------------------------------------
# 📦 Hardware
* ESP32-C3 SuperMini
* IR LEDs 940nm (tested with 9 LEDs in parallel)
* NPN transistor (2N2222, SS8050, BC547 or equivalent)
* Base resistor ~1 kΩ
* LED resistors ~10–47 Ω each
* Push button
* Battery or USB power

---------------------------------------------------------------------------------
## Upgrading IR power with a dedicated driver board
## coming soon
The IR block can be upgraded from a basic transistor circuit to a dedicated high-power IR driver.

### What changes are required?
## wait for the update

### Recommended additions
A high-power IR driver can draw significantly more current than a basic transistor setup.
Recommended:
* Stable 5V power supply
* Dedicated voltage regulator
* 100µF–470µF capacitor between VCC and GND
* Quality 940nm high-power IR LEDs
* Short power wires and solid grounding

The driver can draw up to **500mA** during transmission at full power.

### Performance Improvements
A dedicated IR driver includes a built-in current amplifier compared to a basic NPN transistor.
Compared to a basic transistor circuit:
* Higher drive current
* Better long-range performance
* More consistent pulse shape
* Support for high-power IR emitters
* Better performance in bright ambient light conditions

With a proper driver and 9 high-power LEDs the effective range can exceed **20 meters** under good conditions.

### Why upgrade?
✔ More IR power
✔ Better range
✔ Stronger signal
✔ Support for high-power emitters
✔ Improved reliability

For users who want maximum IR range, a dedicated driver board is a significant upgrade over a basic transistor circuit and is the recommended choice for long-range applications.

---------------------------------------------------------------------------------
## 📝 Adding Custom Codes

IR codes live in `codes.h` stored in PROGMEM to save RAM.

```cpp
static const uint16_t PROGMEM myTimings[] = { /* durations in µs */ };
static const uint8_t  PROGMEM myPairs[]   = { /* mark/space index pairs */ };

// Add to allCodes[]:
{ myTimings, myPairs, sizeof(myPairs), nRepeat, gapMicros },
```

IR code database: [IRDB on GitHub](https://github.com/probonopd/irdb)

---------------------------------------------------------------------------------
*Based on the original TV-B-Gone by Mitch Altman / Limor Fried (Lady Ada)*
