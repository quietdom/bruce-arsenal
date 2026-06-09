🌐 [English](README.md) | [Portugues](docs/README_PT.md) | [Italiano](docs/README_IT.md) | [Francais](docs/README_FR.md) | 🇷🇺 [Русский](docs/README_RU.md) | 🇨🇳 [中文](docs/README_ZH.md) | [Polski](docs/README_PL.md) | [Nederlands](docs/README_NL.md) | [Turkce](docs/README_TR.md) | [Deutsch](docs/README_DE.md) | Tiếng Việt [Tiếng Việt](docs/README_VN.md) | [Espanol](docs/README_ES.md) | [Indonesia](docs/README_ID.md) | 🇸🇦 [العربية](docs/README_AR.md)

<p align="center">
  <img src="media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>Custom offensive security firmware for ESP32 devices</b><br>
  <i>72 tools Full / 55 tools LITE - One firmware - Multiple devices</i>
</p>

---

A modified [Bruce Firmware](https://github.com/pr3y/Bruce) with a custom **Arsenal** module. All original Bruce features remain intact.

## Installation

1. Download the `.bin` from [Releases](../../releases)
2. Flash via [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge) or OTA via Arsenal Dashboard

---

## Arsenal Tools (72 Full / 55 LITE)

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

## Board Compatibility

| Board | Flash | Version |
|-------|-------|---------|
| **ESP32-S3** | | |
| LilyGo T-Embed CC1101 | 16MB | Full |
| LilyGo T-Deck Pro | 16MB | Full |
| LilyGo T-Deck | 16MB | Full |
| LilyGo T-Display S3 | 16MB | Full |
| LilyGo T-Display S3 Touch | 16MB | Full |
| LilyGo T-Display S3 MMC | 16MB | Full |
| LilyGo T-Display S3 Touch MMC | 16MB | Full |
| LilyGo T-HMI | 16MB | Full |
| LilyGo T-LoRa Pager | 16MB | Full |
| LilyGo T-Watch S3 | 16MB | Full |
| M5Stack Cardputer | 8MB | Full |
| M5Stack CoreS3 | 16MB | Full |
| M5Stack StickS3 | 16MB | Full |
| ESP32-S3 DevKitC-1 | 16MB | Full |
| Smoochiee Board | 16MB | Full |
| **ESP32-C5** | | |
| ESP32-C5 | 4MB | Full |
| ESP32-C5 TFT | 4MB | Full |
| **ESP32** | | |
| M5Stack Core 4MB | 4MB | LITE |
| M5Stack Core 16MB | 16MB | LITE |
| M5Stack CPlus 1.1 | 4MB | LITE |
| CYD-2432S028 | 4MB | LITE |
| CYD-2USB | 4MB | LITE |
| CYD-2432W328C | 4MB | LITE |
| CYD-2432W328R | 4MB | LITE |
| CYD-3248S035R | 4MB | LITE |
| CYD-3248S035C | 4MB | LITE |
| Elecrow 2.4B | 4MB | LITE |
| Elecrow 2.8B | 4MB | LITE |
| Elecrow 3.5B | 4MB | LITE |
| LilyGo T-Display TTGO | 4MB | LITE |
| Marauder Mini | 4MB | LITE |
| Marauder V4-V6 | 4MB | LITE |
| Marauder V7 | 4MB | LITE |
| Marauder V6.1 | 4MB | LITE |
| Awok Mini | 4MB | LITE |
| Awok Touch | 4MB | LITE |
| WaveSentry R1 | 4MB | LITE |
| Phantom S024R | 4MB | LITE |
| **Known Issues** | | |
| M5Stack CPlus2 | - | DRAM overflow |
| M5Stack Core2 | - | DRAM overflow |

---

## Remote Dashboard

Arsenal > Dashboard. Connect to `ArsenalNet` (password: `arsenal32`). Open `192.168.4.1`.

## Credits

- [Bruce Firmware](https://github.com/pr3y/Bruce) by pr3y
- Arsenal module by quietdom

## Disclaimer

For authorized security testing and educational purposes only.
