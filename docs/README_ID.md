🌐 [English](../README.md) | [Portugues](README_PT.md) | [Italiano](README_IT.md) | [Francais](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Turkce](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Espanol](README_ES.md) | **Indonesia** | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>🔧 Firmware keamanan ofensif kustom untuk ESP32</b><br>
  <i>⚡ Satu firmware, banyak board — 68 alat</i>
</p>

---

Versi modifikasi dari [Bruce Firmware](https://github.com/pr3y/Bruce) dengan modul **Arsenal** kustom. Semua fitur Bruce asli tetap utuh. Dikompilasi dan diuji di 30 board sebelum setiap rilis.

## 🆕 Apa baru di v1.2.0

**🐛 Diperbaiki** — Membeku RF Listen di T-Embed (bus SPI bersama dilepas saat keluar)·"CC1101 tidak ditemukan"·NRF24"hampir tidak berfungsi"(memverifikasi chip dan daya)·jam_all 4 band·penghitung kirim/gagal sungguhan·kunci WireGuard tidak lagi ke serial·linker deauther.

**✨ Baru** — Universal IR Remote (per merek, kode di flash, tanpa SD)·PMKID Capture (WPA tanpa client)·Deauth Detector (defensif)·Rolljam (jam+tangkap keyfob)·MouseJack asli·simpan/muat target deauth·BLE spammer baru (40 FastPair ID + 18 model Apple).

## 💾 Flash

1. 📥 Unduh `.bin` untuk board kamu dari [Releases](../../releases)
2. 🌐 Buka [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
3. **Connect** → pilih port ESP32 → **Program** → pilih `.bin` → alamat `0x0`
4. 🔄 Tekan **RST** atau colok ulang USB

OTA: hubungkan ke AP (di bawah), buka dashboard, **OTA Update** → pilih `.bin` baru.

CLI: `esptool.py --port COM3 --baud 460800 write_flash 0x0 firmware.bin`

## 📡 Menghubungkan ke dashboard

Ada dua set kredensial — tergantung fitur yang kamu buka:

| | AP name | AP password | Dashboard login | IP |
|---|---------|-------------|-----------------|----|
| **🛡️ Dashboard Arsenal** | `ArsenalNet` | `arsenal32` | `admin` / `arsenal` | `172.0.0.1` |
| **🌐 WebUI Bruce dasar** | `BruceNet` | `brucenet` | `admin` / `bruce` | `172.0.0.1` |

Semua bisa dikonfigurasi di **Pengaturan** di perangkat. Jika default tidak berfungsi, seseorang mengubahnya — flash ulang atau cek Pengaturan → WiFi AP / WebUI. IP-nya **`172.0.0.1`**, bukan `192.168.4.1`.

## 🧰 Alat (68)

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

✅ = berfungsi di Full dan LITE. Sebagian besar Full-only butuh board 16 MB.

## 🖥️ Board

**Full (16 MB):** T-Embed CC1101 · T-Deck / T-Deck Pro · T-Display S3 (all variants) · T-HMI · T-LoRa Pager · T-Watch S3 · M5Stack Cardputer / CoreS3 / StickS3 · ESP32-S3 DevKitC-1 · Smoochiee

**Full (4 MB):** ESP32-C5 / ESP32-C5 TFT

**LITE (4 MB):** M5Stack Core 4/16 MB · CPlus 1.1 · CYD-2432S028/2USB/W328C/W328R · CYD-3248S035R/C · Elecrow 2.4B/2.8B/3.5B · T-Display TTGO · Marauder Mini/V4-V6/V7/V6.1 · Awok Mini/Touch · WaveSentry R1 · Phantom S024R

**⚠️ Masalah diketahui:** M5Stack CPlus2 & Core2 — DRAM overflow, use LITE.

## 🔨 Kompilasi dari sumber

```bash
git clone https://github.com/quietdom/bruce-arsenal
cd bruce-arsenal
pio run -e <board-name>   # nama di platformio.ini
```

## 👥 Kredit

- [Bruce Firmware](https://github.com/pr3y/Bruce) oleh pr3y — basis
- Modul Arsenal + alat oleh quietdom

## ⚖️ Sangkalan

Hanya untuk pengujian keamanan yang berwenang dan tujuan edukasi. Jangan bodoh dengan ini.
