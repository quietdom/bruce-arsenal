🌐 [English](README.md) | [Portugues](docs/README_PT.md) | [Italiano](docs/README_IT.md) | [Francais](docs/README_FR.md) | 🇷🇺 [Русский](docs/README_RU.md) | 🇨🇳 [中文](docs/README_ZH.md) | [Polski](docs/README_PL.md) | [Nederlands](docs/README_NL.md) | [Turkce](docs/README_TR.md) | [Deutsch](docs/README_DE.md) | Tiếng Việt [Tiếng Việt](docs/README_VN.md) | [Espanol](docs/README_ES.md) | [Indonesia](docs/README_ID.md) | 🇸🇦 [العربية](docs/README_AR.md)

<p align="center">
  <img src="media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>Custom offensive-security firmware for ESP32 devices</b><br>
  <i>68 tools — one firmware, many boards</i>
</p>

---

A modified [Bruce Firmware](https://github.com/pr3y/Bruce) with a custom **Arsenal** module bolted on top. Every original Bruce feature stays exactly where it was; the Arsenal layer adds the extra tools below. Built and tested across 30 board environments before every release.

## What's new in v1.2.0

**Fixed**
- **T-Embed RF Listen freeze** — the shared SPI bus was never released on exit, so the display froze after leaving Listen. Fixed, along with an inverted debounce loop and an interrupt attached to an unverified pin.
- **"CC1101 not found"** — on headless and 8-bit displays the init path silently skipped the radio, so the presence check always failed. Now falls back to the default SPI, and pauses the TFT around the probe on shared-bus boards so it doesn't race with display draws.
- **NRF24 "barely works"** — `begin()` can succeed on a loose or clone module that then stops responding. Now verifies the chip with `isChipConnected()` and powers it up properly, or tells you clearly why it didn't.
- **jam_all** — all four bands fixed. Real broadcast deauth with real AP BSSIDs, real sub-GHz dwell time, BLE/NRF24 mutual exclusion (they share one radio), and honest counters instead of a random number bouncing around.
- **Flood/spam counters** — every WiFi and BLE flood module now counts only frames that actually transmitted and shows sent/failed. No more "spammed 5000" when it sent 50.
- **WireGuard** — the private key is no longer printed to the serial console on config load.
- **Deauther** — the implementation that was pasted into the header (and broke linking) is back in the `.cpp` where it belongs, with shared state marked `inline` so it doesn't desync across modules.

**New**
- **Universal IR Remote** — pick a brand (LG, Samsung, Sony, Panasonic so far), get a button grid (Power, Mute, Vol±, CH±, Source, Menu, OK, Up, Down, Back) that sends the right codes from flash. No SD files needed — the codes live in PROGMEM like TV-B-Gone. The `.ir` file system still works for custom captures.
- **PMKID Capture** — client-less WPA. Sends an EAPOL-Start to a chosen AP and pulls the PMKID out of the reply, saving a hashcat-ready HC22000 line to the SD card. No client required.
- **Deauth Detector** — the defensive flip side of the deauther. Sniffs management frames, hops channels, and alerts when *you* are being deauthed, with the source/destination MAC and channel.
- **Rolljam** — the classic keyfob capture. Short jam burst blocks the receiver while the rolling code goes past unheard, then switches to RX to capture it for later replay.
- **Real MouseJack** — the Arsenal entry now runs the actual 934-line implementation instead of a wiring-instructions screen.
- **Saved target lists** — Save/Load deauth target sets to the SD card so they survive a reboot.
- **Refreshed BLE spammer** — unified onto the working code, with 40 real registered FastPair model IDs and 18 Apple Continuity models instead of random bytes that rarely triggered a popup.

## Installation

### Quick flash (recommended)
1. Download the `.bin` for your board from [Releases](../../releases)
2. Open the [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome or Edge)
3. Click **Connect**, pick your ESP32 serial port
4. Click **Program**, choose the `.bin`, set address `0x0`
5. Program, wait, press **RST** or replug USB

### OTA update (after first flash)
1. Connect your phone to `BruceNet` (password: `bruce32`)
2. Open `192.168.4.1` in a browser
3. **OTA Update** → select the new `.bin` → wait for the reboot

### esptool (advanced)
```bash
pip install esptool
esptool.py --port COM3 --baud 460800 write_flash 0x0 firmware.bin
```
Swap `COM3` for your port. Linux/macOS: `/dev/ttyUSB0` or `/dev/cu.usbserial-*`.

---

## Arsenal tools (68)

| # | Tool | Full | LITE | | # | Tool | Full | LITE |
|---|------|:----:|:----:|-|---|------|:----:|:----:|
| 1 | Network Scanner | ✅ | ✅ | | 35 | MAC Rotator | ✅ | ✅ |
| 2 | DHCP Starvation | ✅ | ✅ | | 36 | Channel Hopper | ✅ | ✅ |
| 3 | Karma Attack | ✅ | ✅ | | 37 | Keyfob Logger | ✅ | ❌ |
| 4 | DNS Spoofer | ✅ | ✅ | | 38 | Frequency Scanner | ✅ | ✅ |
| 5 | Auto-Phish Portal | ✅ | ❌ | | 39 | Flipper Import | ✅ | ✅ |
| 6 | Cred Forward | ✅ | ❌ | | 40 | Flipper Detector | ✅ | ❌ |
| 7 | Auth Flood | ✅ | ✅ | | 41 | Hacker Detector | ✅ | ❌ |
| 8 | AP Clone Flood | ✅ | ✅ | | 42 | RF Silence Enforcer | ✅ | ❌ |
| 9 | SSL Strip Lite | ✅ | ❌ | | 43 | ESP-NOW Chat | ✅ | ❌ |
| 10 | DNS Tunnel | ✅ | ✅ | | 44 | ESP-NOW C2 | ✅ | ❌ |
| 11 | WPS PIN Reference | ✅ | ✅ | | 45 | Dead Drop Mesh | ✅ | ❌ |
| 12 | UPnP Port Opener | ✅ | ❌ | | 46 | IR Data Transfer | ✅ | ❌ |
| 13 | Default Cred Scanner | ✅ | ❌ | | 47 | Multi-Device Sync | ✅ | ❌ |
| 14 | Rogue AP Detector | ✅ | ✅ | | 48 | NFC Biz Card | ✅ | ❌ |
| 15 | WiFi Bruteforce | ✅ | ❌ | | 49 | Attack Stats | ✅ | ✅ |
| 16 | WPA Handshake Grabber | ✅ | ❌ | | 50 | Password Generator | ✅ | ✅ |
| 17 | Beacon Flood | ✅ | ❌ | | 51 | Jam All | ✅ | ✅ |
| 18 | Selective Deauth | ✅ | ❌ | | 52 | Combos | ✅ | ✅ |
| 19 | Enhanced Deauth | ✅ | ❌ | | 53 | Scheduler | ✅ | ✅ |
| 20 | ARP Poisoner | ✅ | ❌ | | 54 | Scripts | ✅ | ✅ |
| 21 | BLE Tracker | ✅ | ❌ | | 55 | Session Log | ✅ | ✅ |
| 22 | BT Name Spammer | ✅ | ❌ | | 56 | Config AP | ✅ | ✅ |
| 23 | AirTag Spoofer | ✅ | ❌ | | 57 | Config Dashboard | ✅ | ✅ |
| 24 | Audio Jammer | ✅ | ❌ | | 58 | PIN Lock | ✅ | ✅ |
| 25 | Notification Spoofer | ✅ | ❌ | | 59 | Remote Dashboard | ✅ | ❌ |
| 26 | BT Rickroll | ✅ | ❌ | | 60 | SSID History Logger | ✅ | ✅ |
| 27 | BT Device Profiler | ✅ | ❌ | | 61 | QR Poisoner | ✅ | ✅ |
| 28 | Device Fingerprinter | ✅ | ✅ | | 62 | Auto-Dim | ✅ | ✅ |
| 29 | OPSEC Monitor | ✅ | ✅ | | 63 | OPSEC Background | ✅ | ✅ |
| 30 | OUI Lookup | ✅ | ✅ | | 64 | Deauth Detector | ✅ | ❌ |
| 31 | Probe Log | ✅ | ✅ | | 65 | PMKID Capture | ✅ | ❌ |
| 32 | Banner Grabber | ✅ | ✅ | | 66 | Rolljam | ✅ | ❌ |
| 33 | SmartHome Scan | ✅ | ❌ | | 67 | NRF24 MouseJack | ✅ | ❌ |
| 34 | Channel Chart | ✅ | ✅ | | 68 | Universal IR Remote | ✅ | ✅ |

Full = 16 MB boards. LITE = 4 MB boards (a stripped build that fits the smaller flash).

---

## Board compatibility

| Board | Flash | Build |
|-------|-------|-------|
| **ESP32-S3** | | |
| LilyGo T-Embed CC1101 | 16 MB | Full |
| LilyGo T-Deck Pro | 16 MB | Full |
| LilyGo T-Deck | 16 MB | Full |
| LilyGo T-Display S3 (all variants) | 16 MB | Full |
| LilyGo T-HMI | 16 MB | Full |
| LilyGo T-LoRa Pager | 16 MB | Full |
| LilyGo T-Watch S3 | 16 MB | Full |
| M5Stack Cardputer | 8 MB | Full |
| M5Stack CoreS3 | 16 MB | Full |
| M5Stack StickS3 | 16 MB | Full |
| ESP32-S3 DevKitC-1 | 16 MB | Full |
| Smoochiee Board | 16 MB | Full |
| **ESP32-C5** | | |
| ESP32-C5 | 4 MB | Full |
| ESP32-C5 TFT | 4 MB | Full |
| **ESP32 (4 MB, LITE)** | | |
| M5Stack Core 4 MB | 4 MB | LITE |
| M5Stack Core 16 MB | 16 MB | LITE |
| M5Stack CPlus 1.1 | 4 MB | LITE |
| CYD-2432S028 / 2USB / W328C / W328R | 4 MB | LITE |
| CYD-3248S035R / S035C | 4 MB | LITE |
| Elecrow 2.4B / 2.8B / 3.5B | 4 MB | LITE |
| LilyGo T-Display TTGO | 4 MB | LITE |
| Marauder Mini / V4-V6 / V7 / V6.1 | 4 MB | LITE |
| Awok Mini / Touch | 4 MB | LITE |
| WaveSentry R1 / Phantom S024R | 4 MB | LITE |
| **Known issues** | | |
| M5Stack CPlus2 | — | DRAM overflow — use LITE |
| M5Stack Core2 | — | DRAM overflow — use LITE |

---

## FAQ

**Device boots to a blank screen or keeps restarting.**
You flashed the wrong `.bin` for your board. Re-flash with the matching one. We've all done it.

**"Low memory" when starting a tool.**
Your board doesn't have enough free RAM. Close other running features, or move to a board with more memory (ESP32-S3). The LITE build exists precisely for the 4 MB boards.

**SD card features don't work.**
Insert a FAT32-formatted card before boot. Some boards need the card in the slot at boot time or they skip it entirely.

**WiFi tools crash or find no networks.**
Check the antenna. On boards with an external antenna, check the antenna switch position too. Firmware can't fix physics.

**BLE says "init failed".**
BLE needs NimBLE. If you're building from source, make sure `h2zero/NimBLE-Arduino` is in your `lib_deps`.

**"CC1101 not found".**
Either no CC1101 is connected, the wiring is loose, or (on shared-bus boards) the display was mid-draw during the probe — try again. v1.2.0 hardens this considerably.

**"NRF24 not responding".**
The module isn't acking on SPI. Check wiring and that it's a genuine NRF24L01+ (clones exist and behave badly). A stable 3.3 V supply matters — these brown out easily on TX.

**How do I reach the Remote Dashboard?**
Connect to `BruceNet` (password: `bruce32`) and open `192.168.4.1`.

**Can I build from source?**
Yes. Install [PlatformIO](https://platformio.org/), clone the repo, `pio run -e <board-name>`. Board names are in `platformio.ini`.

---

## Credits

- [Bruce Firmware](https://github.com/pr3y/Bruce) by pr3y — the base this builds on
- Arsenal module and custom tools by quietdom

## Disclaimer

For authorized security testing and educational purposes only. Don't be a jerk with it.
