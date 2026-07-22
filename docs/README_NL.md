🌐 [English](../README.md) | [Portugues](README_PT.md) | [Italiano](README_IT.md) | [Francais](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | **Nederlands** | [Turkce](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Espanol](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>🔧 Aangepaste offensieve-security firmware voor ESP32</b><br>
  <i>⚡ Eén firmware, vele boards — 68 tools</i>
</p>

---

Een aangepaste [Bruce Firmware](https://github.com/pr3y/Bruce) met een eigen **Arsenal**-module. Alle originele Bruce-functies blijven behouden. Voor elke release gecompileerd en getest op 30 boards.

## 🆕 Nieuw in v1.2.0

**🐛 Opgelost** — T-Embed RF Listen vastlopers (gedeelde SPI-bus vrijgegeven bij afsluiten) · "CC1101 niet gevonden" · NRF24 "werkt nauwelijks" (verifieert de chip en voedt hem) · jam_all op 4 banden · echte verzonden/mislukt-tellers · WireGuard-key niet meer naar serial · deauther-linker.

**✨ Nieuw** — Universal IR Remote (per merk, codes in flash, geen SD) · PMKID Capture (WPA zonder client) · Deauth Detector (defensief) · Rolljam (jam+keyfob-capture) · echte MouseJack · deauth-doelen opslaan/laden · BLE-spammer vernieuwd (40 FastPair-ID's + 18 Apple-modellen).

## 💾 Flashen

1. 📥 Download de `.bin` voor je board uit [Releases](../../releases)
2. 🌐 Open de [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
3. **Connect** → kies ESP32-poort → **Program** → kies `.bin` → adres `0x0`
4. 🔄 Druk op **RST** of plug USB opnieuw in

OTA: verbind met het AP (hieronder), open het dashboard, **OTA Update** → kies de nieuwe `.bin`.

CLI: `esptool.py --port COM3 --baud 460800 write_flash 0x0 firmware.bin`

## 📡 Verbinden met het dashboard

Er zijn twee sets inloggegevens — afhankelijk van welke functie je opende:

| | AP name | AP password | Dashboard login | IP |
|---|---------|-------------|-----------------|----|
| **🛡️ Arsenal-dashboard** | `ArsenalNet` | `arsenal32` | `admin` / `arsenal` | `172.0.0.1` |
| **🌐 Bruce-basis-WebUI** | `BruceNet` | `brucenet` | `admin` / `bruce` | `172.0.0.1` |

Alles aanpasbaar via **Instellingen** op het apparaat. Als een standaard niet werkt, heeft iemand hem gewijzigd — herflash of controleer Instellingen → WiFi AP / WebUI. Het IP is **`172.0.0.1`**, niet `192.168.4.1`.

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

✅ = werkt op Full en LITE. De meeste Full-only-tools hebben een 16-MB-board nodig.

## 🖥️ Boards

**Full (16 MB):** T-Embed CC1101 · T-Deck / T-Deck Pro · T-Display S3 (all variants) · T-HMI · T-LoRa Pager · T-Watch S3 · M5Stack Cardputer / CoreS3 / StickS3 · ESP32-S3 DevKitC-1 · Smoochiee

**Full (4 MB):** ESP32-C5 / ESP32-C5 TFT

**LITE (4 MB):** M5Stack Core 4/16 MB · CPlus 1.1 · CYD-2432S028/2USB/W328C/W328R · CYD-3248S035R/C · Elecrow 2.4B/2.8B/3.5B · T-Display TTGO · Marauder Mini/V4-V6/V7/V6.1 · Awok Mini/Touch · WaveSentry R1 · Phantom S024R

**⚠️ Bekende problemen:** M5Stack CPlus2 & Core2 — DRAM overflow, use LITE.

## 🔨 Compileren vanuit broncode

```bash
git clone https://github.com/quietdom/bruce-arsenal
cd bruce-arsenal
pio run -e <board-name>   # namen in platformio.ini
```

## 👥 Credits

- [Bruce Firmware](https://github.com/pr3y/Bruce) door pr3y — de basis
- Arsenal-module + tools door quietdom

## ⚖️ Disclaimer

Alleen voor geautoriseerde securitytests en educatief gebruik. Wees geen idioot ermee.
