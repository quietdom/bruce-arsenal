🌐 [English](README.md) | [Portugues](docs/README_PT.md) | [Italiano](docs/README_IT.md) | [Francais](docs/README_FR.md) | 🇷🇺 [Русский](docs/README_RU.md) | 🇨🇳 [中文](docs/README_ZH.md) | [Polski](docs/README_PL.md) | [Nederlands](docs/README_NL.md) | [Turkce](docs/README_TR.md) | [Deutsch](docs/README_DE.md) | Tiếng Việt [Tiếng Việt](docs/README_VN.md) | [Espanol](docs/README_ES.md) | [Indonesia](docs/README_ID.md) | 🇸🇦 [العربية](docs/README_AR.md)

<p align="center">
  <img src="media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>🔧 Custom offensive-security firmware for ESP32</b><br>
  <i>⚡ One firmware, many boards — 68 tools</i>
</p>

---

A modified [Bruce Firmware](https://github.com/pr3y/Bruce) with a custom **Arsenal** module. All original Bruce features stay intact. Built and tested across 30 boards before every release.

## 🆕 What's new in v1.2.0

**🐛 Fixed** — T-Embed RF Listen freeze (shared SPI bus now released on exit) · "CC1101 not found" (silent init skip + TFT race both fixed) · NRF24 "barely works" (now verifies the chip + powers it up properly) · jam_all across all four bands · honest flood/spam counters (real sent/failed, no fake inflation) · WireGuard private key no longer leaks to serial · deauther linker bomb.

**✨ New** — Universal IR Remote (brand-based, codes in flash, no SD) · PMKID Capture (client-less WPA) · Deauth Detector (defensive) · Rolljam (keyfob jam+capture) · real MouseJack (runs the actual 934-line impl) · Save/Load deauth target lists · refreshed BLE spammer (40 real FastPair IDs + 18 Apple models).

## 💾 Flash

1. 📥 Grab the `.bin` for your board from [Releases](../../releases)
2. 🌐 Open the [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
3. **Connect** → pick your ESP32 port → **Program** → choose the `.bin` → address `0x0`
4. 🔄 Press **RST** or replug USB

OTA: connect to the AP (see below), open the dashboard, **OTA Update** → pick the new `.bin`.

CLI: `esptool.py --port COM3 --baud 460800 write_flash 0x0 firmware.bin`

## 📡 Connecting to the dashboard

There are two credential sets — they depend on which feature you launched:

| | AP name | AP password | Dashboard login | IP |
|---|---------|-------------|-----------------|----|
| **🛡️ Arsenal Dashboard** | `ArsenalNet` | `arsenal32` | `admin` / `arsenal` | `172.0.0.1` |
| **🌐 Base Bruce WebUI** | `BruceNet` | `brucenet` | `admin` / `bruce` | `172.0.0.1` |

All of these are user-configurable from the device's **Settings** menu. If a default doesn't work, someone changed it — reflash or check Settings → WiFi AP / WebUI. The IP is **`172.0.0.1`**, not `192.168.4.1`.

## 🧰 Tools (68)

| | | | | | | |
|---|---|---|---|---|---|---|
| Network Scanner | DHCP Starvation | Karma Attack | DNS Spoofer | Auto-Phish Portal | Cred Forward | Auth Flood |
| AP Clone Flood | SSL Strip Lite | DNS Tunnel | WPS PIN Reference | UPnP Port Opener | Default Cred Scanner | Rogue AP Detector |
| WiFi Bruteforce | WPA Handshake Grabber | Beacon Flood | Selective Deauth | Enhanced Deauth | ARP Poisoner | BLE Tracker |
| BT Name Spammer | AirTag Spoofer | Audio Jammer | Notification Spoofer | BT Rickroll | BT Device Profiler | Device Fingerprinter |
| OPSEC Monitor | OUI Lookup | Probe Log | Banner Grabber | SmartHome Scan | Channel Chart | MAC Rotator |
| Channel Hopper | Keyfob Logger | Frequency Scanner | Flipper Import | Flipper Detector | Hacker Detector | RF Silence Enforcer |
| ESP-NOW Chat | ESP-NOW C2 | Dead Drop Mesh | IR Data Transfer | Multi-Device Sync | NFC Biz Card | Attack Stats |
| Password Generator | Jam All | Combos | Scheduler | Scripts | Session Log | Config AP |
| Config Dashboard | PIN Lock | Remote Dashboard | SSID History Logger | QR Poisoner | Auto-Dim | OPSEC Background |
| Deauth Detector | PMKID Capture | Rolljam | NRF24 MouseJack | Universal IR Remote | | |

✅ = works on both Full and LITE builds. Most Full-only tools need a 16 MB board.

## 🖥️ Boards

**Full (16 MB):** T-Embed CC1101 · T-Deck / T-Deck Pro · T-Display S3 (all variants) · T-HMI · T-LoRa Pager · T-Watch S3 · M5Stack Cardputer / CoreS3 / StickS3 · ESP32-S3 DevKitC-1 · Smoochiee

**Full (4 MB):** ESP32-C5 / ESP32-C5 TFT

**LITE (4 MB):** M5Stack Core 4/16 MB · CPlus 1.1 · CYD-2432S028/2USB/W328C/W328R · CYD-3248S035R/C · Elecrow 2.4B/2.8B/3.5B · T-Display TTGO · Marauder Mini/V4-V6/V7/V6.1 · Awok Mini/Touch · WaveSentry R1 · Phantom S024R

**⚠️ Known issues:** M5Stack CPlus2 & Core2 — DRAM overflow, use LITE.

## 🔨 Build from source

```bash
git clone https://github.com/quietdom/bruce-arsenal
cd bruce-arsenal
pio run -e <board-name>   # names in platformio.ini
```

## 👥 Credits

- [Bruce Firmware](https://github.com/pr3y/Bruce) by pr3y — the base
- Arsenal module + custom tools by quietdom

## ⚖️ Disclaimer

For authorized security testing and educational use only. Don't be a jerk with it.
