<p align="center">
  <img src="media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>Custom offensive security firmware for ESP32 devices</b><br>
  <i>26 attack tools • Mobile dashboard • Background evasion • 60K+ scripts</i>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32--S3-blue?style=flat-square" alt="ESP32-S3">
  <img src="https://img.shields.io/badge/Target-T--Embed%20CC1101-green?style=flat-square" alt="T-Embed">
  <img src="https://img.shields.io/badge/Tools-26+-red?style=flat-square" alt="Tools">
  <img src="https://img.shields.io/badge/Scripts-60K+-purple?style=flat-square" alt="Scripts">
  <img src="https://img.shields.io/badge/Status-Active-brightgreen?style=flat-square" alt="Status">
</p>

---

## What Is This

A heavily modified version of [Bruce Firmware](https://github.com/pr3y/Bruce) with a custom **Arsenal** module bolted on — a full offensive security toolkit accessible from a single menu tab with a crosshair icon.

All original Bruce features remain intact. Arsenal adds 26 new tools, a mobile web dashboard, background evasion services, and a script browser that can load 60K+ payloads from SD card.

---

## Installation

1. Download the `.bin` from [Releases](../../releases)
2. Flash to your device using one of:
   - **USB:** [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
   - **OTA:** After first flash, use Arsenal → Dashboard → upload new .bin from phone
3. Done

---

## Arsenal Menu

```
🎯 Arsenal
├── WiFi Arsenal ──────── 8 offensive WiFi tools
├── BLE Arsenal ───────── 5 Bluetooth attack tools
├── Intelligence ──────── 3 recon/awareness tools
├── Evasion ───────────── 5 stealth/anti-detection tools
├── Combos ────────────── Preset + custom attack chains
├── Scheduler ─────────── Time-delayed attack execution
├── Scripts ───────────── Browse 60K+ payloads from SD
├── Session Log ───────── Activity logging to SD
├── Jam All ───────────── Multi-band simultaneous jamming (custom GUI)
└── Dashboard ─────────── Mobile web UI + file manager + OTA
```

---

## Tools

### WiFi Arsenal

| Tool | What It Does |
|------|-------------|
| **Network Scanner** | ARP sweep + TCP port scan. Shows devices, open ports, vendor names |
| **DHCP Starvation** | Floods DHCP requests with spoofed MACs to exhaust the router's IP pool |
| **Karma Attack** | Answers all WiFi probe requests — nearby devices auto-connect to you |
| **DNS Spoofer** | Intercepts DNS when running as AP. All domains resolve to your portal |
| **Portal Templates** | Loads custom phishing HTML from SD card. Captures credentials to file |
| **Auto-Phish Portal** | Detects what networks victims are looking for, clones the SSID, serves portal |
| **Cred Forward** | Captures portal credentials → auto-connects to real AP → bridges traffic. Full transparent MITM |
| **WiFi Brute Force** | Generates smart wordlist from SSID name (year combos, leet speak, common patterns). Tries live |

### BLE Arsenal

| Tool | What It Does |
|------|-------------|
| **BLE Tracker** | Continuous BLE scan with RSSI distance estimation. Detects AirTags, SmartTags, Tiles following you |
| **Name Spammer** | Floods BLE advertising channels with hundreds of random device names |
| **AirTag Spoofer** | Broadcasts fake Apple FindMy AirTag beacons. Phantom tags appear on nearby iPhones |
| **Audio Jammer** | Disrupts BLE audio connections (AirPods, speakers) with advertisement floods |
| **Notification Spoofer** | Pushes fake device-found popups to Android (Fast Pair), Windows (Swift Pair), and iOS (AirPods proximity) |

### Intelligence

| Tool | What It Does |
|------|-------------|
| **Device Fingerprinter** | Passive WiFi fingerprinting. Identifies device OS/model from probe behavior, OUI, timing |
| **OPSEC Monitor** | Detects if you're being scanned, deauthed, or tracked. Threat level: green/yellow/red |
| **OUI Lookup** | Offline MAC vendor database lookup from SD card |

### Evasion

| Tool | What It Does |
|------|-------------|
| **MAC Rotator** | Randomizes WiFi MAC address on a configurable timer. Adjustable speed |
| **Channel Hopper** | Rapidly switches WiFi channels during operations. Configurable interval |
| **Decoy Traffic** | Emits random beacon/probe frames on rotating channels. Masks real activity in noise |
| **Identity Cloner** | Captures nearby device identities (MAC + probe pattern). Clone one to become invisible |
| **QR Poisoner** | Generates QR codes pointing to your portal. Display on screen, print and place |

### Jam All (Custom GUI)

Full-screen dashboard with per-band toggles, live activity bars, elapsed timer, and one-press kill switch.

| Band | Method |
|------|--------|
| WiFi 2.4GHz | Deauth flood cycling all 14 channels |
| BLE | Advertisement channel flood (37, 38, 39) |
| Sub-GHz | CC1101 continuous TX (315/433/868/915 MHz) |
| NRF24 | 2.4GHz carrier on rotating channels |

### Utilities

| Tool | What It Does |
|------|-------------|
| **Attack Scheduler** | Pick a tool → set delay (0 to 1 hour) → set duration. Auto-starts and stops |
| **Session Log** | Logs every action with timestamps. View/export/clear from menu |
| **Script Browser** | Navigate categorized SD folders. Auto-launches based on file type (.txt/.sub/.ir/.html/.nfc/.js) |
| **Combo Presets** | One-tap attack chains. Built-in: Stealth Mode, Full Attack, Passive Recon. Create custom .txt on SD |
| **Remote Dashboard** | Phone connects to ArsenalNet AP → full mobile web UI with file manager + OTA updates |

---

## Background Services

These run persistently across all menus — not just inside Arsenal.

| Service | Description |
|---------|-------------|
| **OPSEC Dot** | Colored circle on the status bar (green/yellow/red). Always visible, every screen |
| **Always-On Evasion** | MAC rotation + channel hopping + decoy traffic. Runs silently on CPU core 0 |
| **Low Power Mode** | Slows background tasks 5x to save battery. Toggle from Combos menu |
| **Auto-Dim** | Screen brightness drops during active attacks. Restores on stop |

---

## Remote Dashboard

Start Arsenal → Dashboard. Connect your phone to `ArsenalNet` (password: `arsenal32`). Open `192.168.4.1` in browser.

**Features:**
- Start/stop all 26 tools from phone
- Browse, upload, delete files on SD card (`/arsenal/` folder)
- Upload `.bin` firmware for OTA update
- Drag-and-drop script upload to categorized folders
- Live system status (heap, SD space, uptime)

---

## SD Card Layout

```
/arsenal/
├── badusb/       DuckyScript payloads (.txt)
├── subghz/       Sub-GHz signal captures (.sub)
├── ir/           IR remote files (.ir)
├── portals/      Evil portal HTML templates
├── nfc/          NFC card dumps (.nfc)
├── rfid/         125kHz RFID captures (.rfid)
├── ibutton/      iButton key files (.ibutton)
├── scripts/      JavaScript automation (.js)
├── combos/       Custom combo presets (.txt — one feature name per line)
├── logs/         Session logs (auto-created)
└── creds.txt     Captured credentials (auto-created)
```

Compatible with [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts) (60K+ scripts).

---

## Custom Combos

Create a `.txt` file in `/arsenal/combos/` with one feature per line:

```
mac_rotator
channel_hopper
decoy_traffic
ble_tracker
```

It appears in Arsenal → Combos automatically.

---

## Hardware

Primary target: **LilyGo T-Embed CC1101**
- ESP32-S3 (16MB flash, 8MB PSRAM)
- ST7789 320x170 TFT display
- Rotary encoder + button
- CC1101 Sub-GHz radio
- NRF24 2.4GHz radio
- PN532 NFC
- SD card slot
- USB-C (BadUSB HID)
- Speaker + Microphone
- RGB LEDs
- BQ27220 fuel gauge

---

## Credits

- Based on [Bruce Firmware](https://github.com/pr3y/Bruce) by pr3y
- Arsenal module by quietdom
- Script collection compatible with [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts)

---

## Disclaimer

This firmware is for authorized security testing and educational purposes only. Do not use against networks or devices you don't own or have explicit permission to test. The authors are not responsible for misuse.
