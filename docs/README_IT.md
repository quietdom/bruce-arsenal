🌐 [English](../README.md) | [Portugues](README_PT.md) | **Italiano** | [Francais](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Turkce](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Espanol](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>🔧 Firmware di sicurezza offensiva personalizzato per ESP32</b><br>
  <i>⚡ Un firmware, molte schede — 68 strumenti</i>
</p>

---

Una versione modificata del [Bruce Firmware](https://github.com/pr3y/Bruce) con un modulo **Arsenal** personalizzato. Tutte le funzioni originali di Bruce restano intatte. Compilato e testato su 30 schede prima di ogni release.

## 🆕 Novità nella v1.2.0

**🐛 Corretto** — Blocco RF Listen su T-Embed (bus SPI condivisa rilasciata in uscita) · "CC1101 non trovato" · NRF24 "funziona a malapena" (verifica il chip e lo alimenta) · jam_all su 4 bande · contatori reali inviati/falliti · chiave WireGuard non più su seriale · linker deauther.

**✨ Nuovo** — Universal IR Remote (per marca, codici in flash, niente SD) · PMKID Capture (WPA senza client) · Deauth Detector (difensivo) · Rolljam (jam+cattura keyfob) · MouseJack reale · salva/carica target deauth · BLE spammer rinnovato (40 FastPair ID + 18 modelli Apple).

## 💾 Flasbare

1. 📥 Scarica il `.bin` per la tua scheda da [Releases](../../releases)
2. 🌐 Apri l'[ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
3. **Connect** → scegli porta ESP32 → **Program** → scegli `.bin` → indirizzo `0x0`
4. 🔄 Premi **RST** o ricollega USB

OTA: connettiti all'AP (sotto), apri la dashboard, **OTA Update** → scegli il nuovo `.bin`.

CLI: `esptool.py --port COM3 --baud 460800 write_flash 0x0 firmware.bin`

## 📡 Connettersi alla dashboard

Ci sono due set di credenziali — dipendono da quale funzione hai avviato:

| | AP name | AP password | Dashboard login | IP |
|---|---------|-------------|-----------------|----|
| **🛡️ Dashboard Arsenal** | `ArsenalNet` | `arsenal32` | `admin` / `arsenal` | `172.0.0.1` |
| **🌐 WebUI Bruce base** | `BruceNet` | `brucenet` | `admin` / `bruce` | `172.0.0.1` |

Tutto configurabile da **Impostazioni** sul dispositivo. Se un default non funziona, qualcuno lo ha cambiato — riflasha o controlla Impostazioni → WiFi AP / WebUI. L'IP è **`172.0.0.1`**, non `192.168.4.1`.

## 🧰 Strumenti (68)

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

✅ = funziona su Full e LITE. La maggior parte dei Full-only richiede una scheda 16 MB.

## 🖥️ Schede

**Full (16 MB):** T-Embed CC1101 · T-Deck / T-Deck Pro · T-Display S3 (all variants) · T-HMI · T-LoRa Pager · T-Watch S3 · M5Stack Cardputer / CoreS3 / StickS3 · ESP32-S3 DevKitC-1 · Smoochiee

**Full (4 MB):** ESP32-C5 / ESP32-C5 TFT

**LITE (4 MB):** M5Stack Core 4/16 MB · CPlus 1.1 · CYD-2432S028/2USB/W328C/W328R · CYD-3248S035R/C · Elecrow 2.4B/2.8B/3.5B · T-Display TTGO · Marauder Mini/V4-V6/V7/V6.1 · Awok Mini/Touch · WaveSentry R1 · Phantom S024R

**⚠️ Problemi noti:** M5Stack CPlus2 & Core2 — DRAM overflow, use LITE.

## 🔨 Compila da sorgente

```bash
git clone https://github.com/quietdom/bruce-arsenal
cd bruce-arsenal
pio run -e <board-name>   # nomi in platformio.ini
```

## 👥 Crediti

- [Bruce Firmware](https://github.com/pr3y/Bruce) di pr3y — la base
- Modulo Arsenal + strumenti di quietdom

## ⚖️ Disclaimer

Solo per test di sicurezza autorizzati e scopi educativi. Non fare il furbo con questo.
