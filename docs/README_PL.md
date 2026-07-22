🌐 [English](../README.md) | [Portugues](README_PT.md) | [Italiano](README_IT.md) | [Francais](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | **Polski** | [Nederlands](README_NL.md) | [Turkce](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Espanol](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>Niestandardowy firmware ofensywny dla urządzeń ESP32</b><br>
  <i>68 narzędzi — jeden firmware, wiele płytek</i>
</p>

---

Zmodyfikowany [Bruce Firmware](https://github.com/pr3y/Bruce) z własnym modułem **Arsenal**. Wszystkie oryginalne funkcje Bruce pozostają nienaruszone. Kompilowany i testowany na 30 płytkach przed każdym release.

## Nowości w v1.2.0

**Naprawiono:**
- Zamrożenie RF Listen na T-Embed — naprawiono (współdzielona magistrala SPI zwalniana przy wyjściu)
- "CC1101 nie znaleziono" — naprawiono
- NRF24 "ledwo działa" — naprawiono (weryfikuje chip i zasilanie)
- jam_all — naprawiono wszystkie 4 pasma
- Liczniki flood/spam — teraz pokazują prawdziwe wysłane/nieudane
- WireGuard — klucz prywatny nie jest już wypisywany na serial
- Deauther — impl z powrotem w .cpp, linkowanie naprawione

**Nowość:**
- **Universal IR Remote** — pilot TV po marce, kody w flash, bez SD
- **PMKID Capture** — WPA bez klienta, zapisuje HC22000 na SD
- **Deauth Detector** — defensywny, ostrzega gdy Ciebie deautentyfikują
- **Rolljam** — przechwytuje kody przez jam + RX
- **Real MouseJack** — uruchamia teraz prawdziwą 934-wierszową implementację
- **Saved target lists** — zapisuje/wczytuje cele deauth na SD
- **Refreshed BLE spammer** — 40 prawdziwych FastPair ID + 18 modeli Apple

## Instalacja

1. Pobierz `.bin` dla swojej płytki z [Releases](../../releases)
2. Otwórz [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome lub Edge)
3. **Connect**, wybierz port szeregowy ESP32
4. **Program**, wybierz `.bin`, adres `0x0`
5. Programuj, czekaj, naciśnij **RST** lub przełącz USB

---

## Narzędzia Arsenal (68)

| # | Tool | Full | LITE | | # | Tool | Full | LITE |
|---|------|:----:|:----:|-|---|------|:----:|:----:|
| 1 | Network Scanner | ✅ | ✅ | 35 | MAC Rotator | ✅ | ✅ |
| 2 | DHCP Starvation | ✅ | ✅ | 36 | Channel Hopper | ✅ | ✅ |
| 3 | Karma Attack | ✅ | ✅ | 37 | Keyfob Logger | ✅ | ❌ |
| 4 | DNS Spoofer | ✅ | ✅ | 38 | Frequency Scanner | ✅ | ✅ |
| 5 | Auto-Phish Portal | ✅ | ❌ | 39 | Flipper Import | ✅ | ✅ |
| 6 | Cred Forward | ✅ | ❌ | 40 | Flipper Detector | ✅ | ❌ |
| 7 | Auth Flood | ✅ | ✅ | 41 | Hacker Detector | ✅ | ❌ |
| 8 | AP Clone Flood | ✅ | ✅ | 42 | RF Silence Enforcer | ✅ | ❌ |
| 9 | SSL Strip Lite | ✅ | ❌ | 43 | ESP-NOW Chat | ✅ | ❌ |
| 10 | DNS Tunnel | ✅ | ✅ | 44 | ESP-NOW C2 | ✅ | ❌ |
| 11 | WPS PIN Reference | ✅ | ✅ | 45 | Dead Drop Mesh | ✅ | ❌ |
| 12 | UPnP Port Opener | ✅ | ❌ | 46 | IR Data Transfer | ✅ | ❌ |
| 13 | Default Cred Scanner | ✅ | ❌ | 47 | Multi-Device Sync | ✅ | ❌ |
| 14 | Rogue AP Detector | ✅ | ✅ | 48 | NFC Biz Card | ✅ | ❌ |
| 15 | WiFi Bruteforce | ✅ | ❌ | 49 | Attack Stats | ✅ | ✅ |
| 16 | WPA Handshake Grabber | ✅ | ❌ | 50 | Password Generator | ✅ | ✅ |
| 17 | Beacon Flood | ✅ | ❌ | 51 | Jam All | ✅ | ✅ |
| 18 | Selective Deauth | ✅ | ❌ | 52 | Combos | ✅ | ✅ |
| 19 | Enhanced Deauth | ✅ | ❌ | 53 | Scheduler | ✅ | ✅ |
| 20 | ARP Poisoner | ✅ | ❌ | 54 | Scripts | ✅ | ✅ |
| 21 | BLE Tracker | ✅ | ❌ | 55 | Session Log | ✅ | ✅ |
| 22 | BT Name Spammer | ✅ | ❌ | 56 | Config AP | ✅ | ✅ |
| 23 | AirTag Spoofer | ✅ | ❌ | 57 | Config Dashboard | ✅ | ✅ |
| 24 | Audio Jammer | ✅ | ❌ | 58 | PIN Lock | ✅ | ✅ |
| 25 | Notification Spoofer | ✅ | ❌ | 59 | Remote Dashboard | ✅ | ❌ |
| 26 | BT Rickroll | ✅ | ❌ | 60 | SSID History Logger | ✅ | ✅ |
| 27 | BT Device Profiler | ✅ | ❌ | 61 | QR Poisoner | ✅ | ✅ |
| 28 | Device Fingerprinter | ✅ | ✅ | 62 | Auto-Dim | ✅ | ✅ |
| 29 | OPSEC Monitor | ✅ | ✅ | 63 | OPSEC Background | ✅ | ✅ |
| 30 | OUI Lookup | ✅ | ✅ | 64 | Deauth Detector | ✅ | ❌ |
| 31 | Probe Log | ✅ | ✅ | 65 | PMKID Capture | ✅ | ❌ |
| 32 | Banner Grabber | ✅ | ✅ | 66 | Rolljam | ✅ | ❌ |
| 33 | SmartHome Scan | ✅ | ❌ | 67 | NRF24 MouseJack | ✅ | ❌ |
| 34 | Channel Chart | ✅ | ✅ | 68 | Universal IR Remote | ✅ | ✅ |

---

## Kompatybilność płytek

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
| **Znane problemy** | | |
| M5Stack CPlus2 | — | DRAM overflow — use LITE |
| M5Stack Core2 | — | DRAM overflow — use LITE |

---

## Pulpit zdalny

Arsenal > Dashboard. Połącz z `BruceNet` (hasło: `bruce32`). Otwórz `192.168.4.1`.

## Podziękowania

- [Bruce Firmware](https://github.com/pr3y/Bruce) od pr3y — podstawa tego projektu
- Moduł Arsenal i własne narzędzia od quietdom

## Zastrzeżenie

Tylko do autoryzowanych testów bezpieczeństwa i celów edukacyjnych. Nie bądź idiotą z tym.
