🌐 [English](../README.md) | [Portugues](README_PT.md) | [Italiano](README_IT.md) | [Francais](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Turkce](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | **Espanol** | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>🔧 Firmware de seguridad ofensiva personalizado para ESP32</b><br>
  <i>⚡ Un firmware, muchas placas — 68 herramientas</i>
</p>

---

Una versión modificada del [Bruce Firmware](https://github.com/pr3y/Bruce) con un módulo **Arsenal** personalizado. Todas las funciones originales de Bruce permanecen intactas. Compilado y probado en 30 placas antes de cada release.

## 🆕 Novedades en v1.2.0

**🐛 Corregido** — Congelación RF Listen en T-Embed (bus SPI compartido liberado al salir) · "CC1101 no encontrado" · NRF24 "apenas funciona" (verifica el chip y lo alimenta) · jam_all en 4 bandas · contadores reales enviados/fallidos · clave WireGuard ya no en el serial · linker deauther.

**✨ Nuevo** — Universal IR Remote (por marca, códigos en flash, sin SD) · PMKID Capture (WPA sin cliente) · Deauth Detector (defensivo) · Rolljam (jam+captura keyfob) · MouseJack real · guardar/cargar objetivos deauth · BLE spammer renovado (40 FastPair IDs + 18 modelos Apple).

## 💾 Flashear

1. 📥 Descarga el `.bin` de tu placa desde [Releases](../../releases)
2. 🌐 Abre el [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
3. **Connect** → elige puerto ESP32 → **Program** → elige `.bin` → dirección `0x0`
4. 🔄 Pulsa **RST** o reconecta el USB

OTA: conéctate al AP (abajo), abre el dashboard, **OTA Update** → elige el nuevo `.bin`.

CLI: `esptool.py --port COM3 --baud 460800 write_flash 0x0 firmware.bin`

## 📡 Conectarse al dashboard

Hay dos conjuntos de credenciales — dependen de qué función abriste:

| | AP name | AP password | Dashboard login | IP |
|---|---------|-------------|-----------------|----|
| **🛡️ Dashboard Arsenal** | `ArsenalNet` | `arsenal32` | `admin` / `arsenal` | `172.0.0.1` |
| **🌐 WebUI Bruce base** | `BruceNet` | `brucenet` | `admin` / `bruce` | `172.0.0.1` |

Todo configurable desde **Ajustes** en el dispositivo. Si un valor por defecto no funciona, alguien lo cambió — reflashea o revisa Ajustes → WiFi AP / WebUI. La IP es **`172.0.0.1`**, no `192.168.4.1`.

## 🧰 Herramientas (68)

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

✅ = funciona en Full y LITE. La mayoría de Full-only necesitan una placa de 16 MB.

## 🖥️ Placas

**Full (16 MB):** T-Embed CC1101 · T-Deck / T-Deck Pro · T-Display S3 (all variants) · T-HMI · T-LoRa Pager · T-Watch S3 · M5Stack Cardputer / CoreS3 / StickS3 · ESP32-S3 DevKitC-1 · Smoochiee

**Full (4 MB):** ESP32-C5 / ESP32-C5 TFT

**LITE (4 MB):** M5Stack Core 4/16 MB · CPlus 1.1 · CYD-2432S028/2USB/W328C/W328R · CYD-3248S035R/C · Elecrow 2.4B/2.8B/3.5B · T-Display TTGO · Marauder Mini/V4-V6/V7/V6.1 · Awok Mini/Touch · WaveSentry R1 · Phantom S024R

**⚠️ Problemas conocidos:** M5Stack CPlus2 & Core2 — DRAM overflow, use LITE.

## 🔨 Compilar desde el código

```bash
git clone https://github.com/quietdom/bruce-arsenal
cd bruce-arsenal
pio run -e <board-name>   # nombres en platformio.ini
```

## 👥 Créditos

- [Bruce Firmware](https://github.com/pr3y/Bruce) por pr3y — la base
- Módulo Arsenal + herramientas por quietdom

## ⚖️ Aviso

Solo para pruebas de seguridad autorizadas y fines educativos. No seas idiota con esto.
