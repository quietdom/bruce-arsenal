🌐 [English](../README.md) | [Portugues](README_PT.md) | [Italiano](README_IT.md) | [Francais](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Turkce](README_TR.md) | **Deutsch** | [Tiếng Việt](README_VN.md) | [Espanol](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/artworks/bruce-working.jpg" alt="Bruce Arsenal" width="500">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>Benutzerdefinierte Offensiv-Sicherheits-Firmware für ESP32</b><br>
  <i>Eine Firmware, viele Boards — 68 Werkzeuge</i>
</p>

---

Eine modifizierte [Bruce Firmware](https://github.com/pr3y/Bruce) mit einem benutzerdefinierten **Arsenal**-Modul. Alle originalen Bruce-Funktionen bleiben erhalten. Vor jedem Release auf 30 Boards kompiliert und getestet.

## Neu in v1.2.0

**Behoben** — T-Embed RF Listen Einfrieren (geteilter SPI-Bus beim Beenden freigegeben) · „CC1101 nicht gefunden“ · NRF24 „funktioniert kaum“ (prüft den Chip und versorgt ihn) · jam_all auf 4 Bändern · echte Zähler gesendet/fehlgeschlagen · WireGuard-Key nicht mehr auf Serial · Deauther-Linker.

**Neu** — Universal IR Remote (nach Marke, Codes im Flash, kein SD) · PMKID Capture (WPA ohne Client) · Deauth Detector (defensiv) · Rolljam (Jam+Keyfob-Capture) · echtes MouseJack · Deauth-Ziele speichern/laden · BLE-Spammer erneuert (40 FastPair-IDs + 18 Apple-Modelle).

## Flashen

1. Lade die `.bin` für dein Board aus [Releases](../../releases)
2. Öffne den [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
3. **Connect** → wähle ESP32-Port → **Program** → wähle `.bin` → Adresse `0x0`
4. **RST** drücken oder USB neu anschließen

OTA: verbinde dich mit dem AP (unten), öffne das Dashboard, **OTA Update** → wähle die neue `.bin`.

CLI: `esptool.py --port COM3 --baud 460800 write_flash 0x0 firmware.bin`

## Mit dem Dashboard verbinden

Es gibt zwei Credentials-Sätze — je nachdem, welche Funktion du gestartet hast:

| | AP name | AP password | Dashboard login | IP |
|---|---------|-------------|-----------------|----|
| **Arsenal-Dashboard** | `ArsenalNet` | `arsenal32` | `admin` / `arsenal` | `172.0.0.1` |
| **Bruce-Basis-WebUI** | `BruceNet` | `brucenet` | `admin` / `bruce` | `172.0.0.1` |

Alles konfigurierbar unter **Einstellungen** am Gerät. Wenn ein Default nicht geht, hat jemand ihn geändert — neu flashen oder Einstellungen → WiFi AP / WebUI prüfen. Die IP ist **`172.0.0.1`**, nicht `192.168.4.1`.

## Werkzeuge (68)

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

✅ = funktioniert auf Full und LITE. Die meisten Full-only brauchen ein 16-MB-Board.

## Boards

**Full (16 MB):** T-Embed CC1101 · T-Deck / T-Deck Pro · T-Display S3 (all variants) · T-HMI · T-LoRa Pager · T-Watch S3 · M5Stack Cardputer / CoreS3 / StickS3 · ESP32-S3 DevKitC-1 · Smoochiee

**Full (4 MB):** ESP32-C5 / ESP32-C5 TFT

**LITE (4 MB):** M5Stack Core 4/16 MB · CPlus 1.1 · CYD-2432S028/2USB/W328C/W328R · CYD-3248S035R/C · Elecrow 2.4B/2.8B/3.5B · T-Display TTGO · Marauder Mini/V4-V6/V7/V6.1 · Awok Mini/Touch · WaveSentry R1 · Phantom S024R

**Bekannte Probleme:** M5Stack CPlus2 & Core2 — DRAM overflow, use LITE.

## Aus dem Code kompilieren

```bash
git clone https://github.com/quietdom/bruce-arsenal
cd bruce-arsenal
pio run -e <board-name>   # Namen in platformio.ini
```

## Credits

- [Bruce Firmware](https://github.com/pr3y/Bruce) von pr3y — die Basis
- Arsenal-Modul + Werkzeuge von quietdom

## Haftungsausschluss

Nur für autorisierte Sicherheitstests und Bildungszwecke. Sei kein Idiot damit.
