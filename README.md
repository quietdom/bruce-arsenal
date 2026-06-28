🌐 [English](README.md) | [Portugues](docs/README_PT.md) | [Italiano](docs/README_IT.md) | [Francais](docs/README_FR.md) | 🇷🇺 [Русский](docs/README_RU.md) | 🇨🇳 [中文](docs/README_ZH.md) | [Polski](docs/README_PL.md) | [Nederlands](docs/README_NL.md) | [Turkce](docs/README_TR.md) | [Deutsch](docs/README_DE.md) | Tiếng Việt [Tiếng Việt](docs/README_VN.md) | [Espanol](docs/README_ES.md) | [Indonesia](docs/README_ID.md) | 🇸🇦 [العربية](docs/README_AR.md)

<p align="center">
  <img src="media/bruce_banner.jpg" alt="Bruce Firmware" width="700">
</p>

<h1 align="center">🎯 Bruce Firmware</h1>

<p align="center">
  <b>Custom offensive security firmware for ESP32 devices</b><br>
  <i>73 tools Full / 56 tools LITE - One firmware - Multiple devices</i>
</p>

---

A modified [Bruce Firmware](https://github.com/pr3y/Bruce) with additional tools and features. All original Bruce features remain intact.

## Installation

### Quick Flash (Recommended)

1. Download the `.bin` for your board from [Releases](../../releases)
2. Go to [ESP Web Flasher](https://espressif.github.io/esptool-js/) (use Chrome or Edge)
3. Click **Connect**, select your ESP32 serial port
4. Click **Program**, select the `.bin` file, set address to `0x0`
5. Click **Program** and wait for completion
6. Press **RST** on your device or unplug/replug USB

### OTA Update (After First Flash)

1. Connect your phone to `BruceNet` (password: `bruce32`)
2. Open `192.168.4.1` in your browser
3. Go to **OTA Update**, select the `.bin` file
4. Wait for upload and automatic reboot

### Flash via esptool (Advanced)

```bash
pip install esptool
esptool.py --port COM3 --baud 460800 write_flash 0x0 firmware.bin
```
Replace `COM3` with your serial port. On Linux use `/dev/ttyUSB0`.

---

## Additional Tools (72 Full / 55 LITE)

| # | Tool | Full | LITE | | # | Tool | Full | LITE |
|---|------|:----:|:----:|-|---|------|:----:|:----:|
| 1 | Network Scanner | ✅ | ✅ | | 37 | Keyfob Logger | ✅ | ❌ |
| 2 | DHCP Starvation | ✅ | ✅ | | 38 | Frequency Scanner | ✅ | ✅ |
| 3 | Karma Attack | ✅ | ✅ | | 39 | Flipper Import | ✅ | ✅ |
| 4 | DNS Spoofer | ✅ | ✅ | | 40 | Flipper Detector | ✅ | ❌ |
| 5 | Auto-Phish Portal | ✅ | ❌ | | 41 | Hacker Detector | ✅ | ❌ |
| 6 | Cred Forward | ✅ | ❌ | | 42 | RF Silence Enforcer | ✅ | ❌ |
| 7 | Auth Flood | ✅ | ✅ | | 43 | ESP-NOW Chat | ✅ | ❌ |
| 8 | AP Clone Flood | ✅ | ✅ | | 44 | ESP-NOW C2 | ✅ | ❌ |
| 9 | SSL Strip Lite | ✅ | ❌ | | 45 | Dead Drop Mesh | ✅ | ❌ |
| 10 | DNS Tunnel | ✅ | ✅ | | 46 | IR Data Transfer | ✅ | ❌ |
| 11 | WPS PIN Attack | ✅ | ✅ | | 47 | Multi-Device Sync | ✅ | ❌ |
| 12 | UPnP Port Opener | ✅ | ❌ | | 48 | NFC Biz Card | ✅ | ❌ |
| 13 | Default Cred Scanner | ✅ | ❌ | | 49 | Attack Stats | ✅ | ✅ |
| 14 | Rogue AP Detector | ✅ | ✅ | | 50 | Password Generator | ✅ | ✅ |
| 15 | WiFi Bruteforce | ✅ | ❌ | | 51 | Jam All | ✅ | ✅ |
| 16 | WPA Handshake Grabber | ✅ | ❌ | | 52 | Combos | ✅ | ✅ |
| 17 | Beacon Flood | ✅ | ❌ | | 53 | Scheduler | ✅ | ✅ |
| 18 | Selective Deauth | ✅ | ❌ | | 54 | Scripts | ✅ | ✅ |
| 19 | ARP Poisoner | ✅ | ❌ | | 55 | Session Log | ✅ | ✅ |
| 20 | BLE Tracker | ✅ | ❌ | | 56 | Config AP | ✅ | ✅ |
| 21 | BT Name Spammer | ✅ | ❌ | | 57 | Config Dashboard | ✅ | ✅ |
| 22 | AirTag Spoofer | ✅ | ❌ | | 58 | PIN Lock | ✅ | ✅ |
| 23 | Audio Jammer | ✅ | ❌ | | 59 | Remote Dashboard | ✅ | ❌ |
| 24 | Notification Spoofer | ✅ | ❌ | | 60 | SSID History Logger | ✅ | ✅ |
| 25 | BT Rickroll | ✅ | ❌ | | 61 | QR Poisoner | ✅ | ✅ |
| 26 | BT Device Profiler | ✅ | ❌ | | 62 | Auto-Dim | ✅ | ✅ |
| 27 | Device Fingerprinter | ✅ | ✅ | | 63 | OPSEC Background | ✅ | ✅ |
| 28 | OPSEC Monitor | ✅ | ✅ | | 64 | Network Scanner (Full) | ✅ | ✅ |
| 29 | OUI Lookup | ✅ | ✅ | | 65 | DHCP Starvation (Full) | ✅ | ✅ |
| 30 | Probe Log | ✅ | ✅ | | 66 | Karma Attack (Full) | ✅ | ✅ |
| 31 | Banner Grabber | ✅ | ✅ | | 67 | DNS Spoofer (Full) | ✅ | ✅ |
| 32 | SmartHome Scan | ✅ | ❌ | | 68 | Auth Flood (Full) | ✅ | ✅ |
| 33 | Channel Chart | ✅ | ✅ | | 69 | AP Clone Flood (Full) | ✅ | ✅ |
| 34 | People Counter | ✅ | ❌ | | 70 | DNS Tunnel (Full) | ✅ | ✅ |
| 35 | MAC Rotator | ✅ | ✅ | | 71 | WPS PIN Attack (Full) | ✅ | ✅ |
| 36 | Channel Hopper | ✅ | ✅ | | 72 | Rogue AP Detector (Full) | ✅ | ✅ |

---

## Not Compatible

- **M5Stack CPlus2** - DRAM overflow (use LITE version)
- **M5Stack Core2** - DRAM overflow (use LITE version)

All other supported Bruce boards work with Full or LITE depending on flash/RAM.

---

## FAQ

**Q: Device boots to a blank screen or keeps restarting**
A: Flash again with the correct `.bin` for your board. Make sure you selected the right serial port.

**Q: "Low memory" error when starting a tool**
A: Your board doesn't have enough free RAM. Close other running features or use a board with more memory (ESP32-S3).

**Q: SD card features don't work**
A: Insert a formatted FAT32 SD card before booting. Some boards require SD card in the slot at boot time.

**Q: WiFi tools crash or don't find networks**
A: Make sure WiFi antenna is connected. On boards with external antenna, check the antenna switch.

**Q: BLE features say "init failed"**
A: BLE requires NimBLE library. If building from source, make sure `h2zero/NimBLE-Arduino` is in your `lib_deps`.

**Q: How do I access the Remote Dashboard?**
A: Connect phone to WiFi `BruceNet` (password: `bruce32`), open `192.168.4.1` in browser.

**Q: How do I update firmware OTA?**
A: Connect to `BruceNet`, open dashboard, go to OTA Update, select new `.bin` file.

**Q: Can I build from source?**
A: Yes. Install [PlatformIO](https://platformio.org/), clone this repo, run `pio run -e <board-name>`. Board names are in `platformio.ini`.

---

## Remote Dashboard

Connect to `BruceNet` (password: `bruce32`). Open `192.168.4.1`.

## Credits

- [Bruce Firmware](https://github.com/pr3y/Bruce) by pr3y
- Custom tools by quietdom

## Disclaimer

For authorized security testing and educational purposes only.
