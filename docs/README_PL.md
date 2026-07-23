🌐 [English](../README.md) | [Portugues](README_PT.md) | [Italiano](README_IT.md) | [Francais](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | **Polski** | [Nederlands](README_NL.md) | [Turkce](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Espanol](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>🔧 Niestandardowy firmware ofensywny dla ESP32</b><br>
  <i>⚡ Jeden firmware, wiele płytek — 68 narzędzi</i>
</p>

---

Zmodyfikowany [Bruce Firmware](https://github.com/pr3y/Bruce) z własnym modułem **Arsenal**. Wszystkie oryginalne funkcje Bruce pozostają nienaruszone. Kompilowany i testowany na 30 płytkach przed każdym release.

## 🆕 Nowości w v1.2.0

**🐛 Naprawiono** — Zamrożenie RF Listen na T-Embed (współdzielona magistrala SPI zwalniana przy wyjściu) · "CC1101 nie znaleziono" · NRF24 "ledwo działa" (weryfikuje chip i zasilanie) · jam_all na 4 pasmach · prawdziwe liczniki wysłane/nieudane · klucz WireGuard nie na serialu · linker deauther.

**✨ Nowość** — Universal IR Remote (po marce, kody w flash, bez SD) · PMKID Capture (WPA bez klienta) · Deauth Detector (defensywny) · Rolljam (jam+przechwyt keyfob) · prawdziwy MouseJack · zapis/wczytywanie celów deauth · odświeżony BLE spammer (40 FastPair ID + 18 modeli Apple).

## 💾 Wgranie

1. 📥 Pobierz `.bin` dla swojej płytki z [Releases](../../releases)
2. 🌐 Otwórz [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
3. **Connect** → wybierz port ESP32 → **Program** → wybierz `.bin` → adres `0x0`
4. 🔄 Naciśnij **RST** lub przełącz USB

OTA: połącz z AP (poniżej), otwórz pulpit, **OTA Update** → wybierz nowy `.bin`.

CLI: `esptool.py --port COM3 --baud 460800 write_flash 0x0 firmware.bin`

## 📡 Łączenie z pulpitem

Są dwa zestawy poświadczeń — zależą od uruchomionej funkcji:

| | AP name | AP password | Dashboard login | IP |
|---|---------|-------------|-----------------|----|
| **🛡️ Pulpit Arsenal** | `ArsenalNet` | `arsenal32` | `admin` / `arsenal` | `172.0.0.1` |
| **🌐 WebUI Bruce (bazowa)** | `BruceNet` | `brucenet` | `admin` / `bruce` | `172.0.0.1` |

Wszystko konfigurowalne w **Ustawieniach** na urządzeniu. Jeśli domyślne nie działa, ktoś je zmienił — wgraj ponownie lub sprawdź Ustawienia → WiFi AP / WebUI. IP to **`172.0.0.1`**, nie `192.168.4.1`.

## 🧰 Narzędzia (68)

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

✅ = działa na Full i LITE. Większość Full-only wymaga płytki 16 MB.

## 🖥️ Płytki

**Full (16 MB):** T-Embed CC1101 · T-Deck / T-Deck Pro · T-Display S3 (all variants) · T-HMI · T-LoRa Pager · T-Watch S3 · M5Stack Cardputer / CoreS3 / StickS3 · ESP32-S3 DevKitC-1 · Smoochiee

**Full (4 MB):** ESP32-C5 / ESP32-C5 TFT

**LITE (4 MB):** M5Stack Core 4/16 MB · CPlus 1.1 · CYD-2432S028/2USB/W328C/W328R · CYD-3248S035R/C · Elecrow 2.4B/2.8B/3.5B · T-Display TTGO · Marauder Mini/V4-V6/V7/V6.1 · Awok Mini/Touch · WaveSentry R1 · Phantom S024R

**⚠️ Znane problemy:** M5Stack CPlus2 & Core2 — DRAM overflow, use LITE.

## 🔨 Kompilacja ze źródeł

```bash
git clone https://github.com/quietdom/bruce-arsenal
cd bruce-arsenal
pio run -e <board-name>   # nazwy w platformio.ini
```

## 👥 Podziękowania

- [Bruce Firmware](https://github.com/pr3y/Bruce) od pr3y — podstawa
- Moduł Arsenal + narzędzia od quietdom

## ⚖️ Zastrzeżenie

Tylko do autoryzowanych testów bezpieczeństwa i celów edukacyjnych. Nie bądź idiotą z tym.
