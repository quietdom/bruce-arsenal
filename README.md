# Bruce Arsenal — Custom Firmware Mod

<p align="center">
  <img src="media/bruce_banner.jpg" alt="Bruce Firmware" width="600">
</p>

**Modified firmware based on [Bruce](https://github.com/pr3y/Bruce) with a custom Arsenal module — 26+ offensive security tools, a mobile web dashboard, script browser, and background evasion services.**

Built for the **LilyGo T-Embed CC1101** (ESP32-S3).

---

## Arsenal Features

### WiFi Arsenal
| Tool | Description |
|------|-------------|
| Network Scanner | ARP sweep + TCP port scan on connected network |
| DHCP Starvation | Flood DHCP requests to exhaust router's IP pool |
| Karma Attack | Answer all probe requests — devices auto-connect to you |
| DNS Spoofer | Redirect domains when running as AP |
| Portal Templates | SD-loaded phishing portals with credential logging |
| Auto-Phish Portal | Detect nearby SSIDs from probes, clone and serve matching portal |
| Cred Forward | Capture creds, auto-try on real network, bridge traffic (transparent MITM) |
| WiFi Brute Force | Smart wordlist generator from SSID name + live connection attempts |

### BLE Arsenal
| Tool | Description |
|------|-------------|
| BLE Tracker | Scan nearby BLE devices, RSSI tracking, detect AirTags/SmartTags following you |
| Name Spammer | Flood BLE advertisements with funny/random device names |
| AirTag Spoofer | Broadcast phantom AirTag beacons (appear on Find My) |
| Audio Jammer | Disrupt BLE headphones/speakers with connection floods |
| Notification Spoofer | Push fake popups to Android (Fast Pair), Windows (Swift Pair), iOS (AirPods) |

### Intelligence
| Tool | Description |
|------|-------------|
| Device Fingerprinter | Identify device OS/model from probe request patterns and OUI |
| OPSEC Monitor | Detect if someone is deauthing/scanning YOU — green/yellow/red threat level |
| OUI Lookup | Offline MAC vendor lookup from SD database |

### Evasion
| Tool | Description |
|------|-------------|
| MAC Rotator | Auto-randomize WiFi MAC on configurable timer |
| Channel Hopper | Rapid channel switching to avoid spectrum analyzers |
| Decoy Traffic | Emit noise frames on multiple channels to mask real activity |
| Identity Cloner | Copy a nearby device's MAC + probe pattern to blend in |
| QR Poisoner | Generate and display malicious QR codes pointing to your portal |

### Nuclear
| Tool | Description |
|------|-------------|
| Jam All | Simultaneous multi-band jamming with custom GUI — WiFi, BLE, Sub-GHz, NRF24 |

### Utilities
| Tool | Description |
|------|-------------|
| Attack Scheduler | Set delay + duration for any attack ("run karma in 5 min for 2 min") |
| Session Log | Log all activity with timestamps to SD card |
| Script Browser | Browse and launch 60K+ scripts from SD (BadUSB, Sub-GHz, IR, NFC, RFID, JS) |
| Combo Presets | One-tap attack chains (Stealth Mode, Full Attack, Passive Recon) + custom combos from SD |
| Remote Dashboard | Mobile web UI — control all tools from phone, upload scripts, OTA firmware update |

---

## Background Services (always running)

- **OPSEC Dot** — Green/yellow/red circle on status bar, visible on ALL screens
- **Always-On Evasion** — MAC rotate + channel hop + decoy traffic running silently
- **Low Power Mode** — Slows background tasks to save battery (~65% less overhead)
- **Auto-Dim** — Screen dims during active attacks for stealth

---

## SD Card Structure

Place scripts in these folders on your SD card:

```
/arsenal/
├── badusb/       — DuckyScript .txt payloads
├── subghz/       — Sub-GHz .sub signal files
├── ir/           — IR remote .ir captures
├── portals/      — Evil portal .html templates
├── nfc/          — NFC dump files
├── rfid/         — RFID captures
├── ibutton/      — iButton key files
├── scripts/      — JavaScript automation scripts
├── combos/       — Custom combo preset .txt files
├── logs/         — Session logs (auto-created)
└── creds.txt     — Captured credentials (auto-created)
```

---

## Flashing

### USB (first time)
```
pio run -e lilygo-t-embed-cc1101 --target upload
```

### OTA (after first flash)
Start Arsenal → Dashboard → connect phone to ArsenalNet → upload .bin from browser

### Web Flasher
Use [ESP Web Flasher](https://espressif.github.io/esptool-js/) in Chrome — plug in USB, select .bin, flash.

---

## Based On

This is a modified version of [Bruce Firmware](https://github.com/pr3y/Bruce) by pr3y. All original Bruce features (WiFi attacks, BLE, RF, IR, NFC, RFID, GPS, LoRa, BadUSB, JavaScript interpreter, etc.) are included alongside the Arsenal module.

---

## License

Same as upstream Bruce — see [LICENSE](LICENSE).
