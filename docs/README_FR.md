🌐 [English](../README.md) | [Portugues](README_PT.md) | [Italiano](README_IT.md) | **Francais** | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Turkce](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Espanol](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>🔧 Firmware de sécurité offensive personnalisé pour ESP32</b><br>
  <i>⚡ Un firmware, de nombreuses cartes — 68 outils</i>
</p>

---

Une version modifiée du [Bruce Firmware](https://github.com/pr3y/Bruce) avec un module **Arsenal** personnalisé. Toutes les fonctions Bruce d'origine restent intactes. Compilé et testé sur 30 cartes avant chaque release.

## 🆕 Nouveautés v1.2.0

**🐛 Corrigé** — Freeze RF Listen sur T-Embed (bus SPI partagé libéré en sortie) · « CC1101 introuvable » · NRF24 « fonctionne à peine » (vérifie la puce et l'alimente) · jam_all sur 4 bandes · compteurs réels envoyés/échoués · clé WireGuard plus sur le port série · linker deauther.

**✨ Nouveau** — Universal IR Remote (par marque, codes en flash, sans SD) · PMKID Capture (WPA sans client) · Deauth Detector (défensif) · Rolljam (jam+capture keyfob) · MouseJack réel · sauve/charge cibles deauth · BLE spammer rafraîchi (40 FastPair ID + 18 modèles Apple).

## 💾 Flasher

1. 📥 Téléchargez le `.bin` de votre carte depuis [Releases](../../releases)
2. 🌐 Ouvrez l'[ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
3. **Connect** → choisissez port ESP32 → **Program** → choisissez `.bin` → adresse `0x0`
4. 🔄 Appuyez sur **RST** ou rebranchez l'USB

OTA : connectez-vous à l'AP (ci-dessous), ouvrez le tableau de bord, **OTA Update** → choisissez le nouveau `.bin`.

CLI : `esptool.py --port COM3 --baud 460800 write_flash 0x0 firmware.bin`

## 📡 Se connecter au tableau de bord

Il y a deux jeux d'identifiants — selon la fonction lancée :

| | AP name | AP password | Dashboard login | IP |
|---|---------|-------------|-----------------|----|
| **🛡️ Tableau de bord Arsenal** | `ArsenalNet` | `arsenal32` | `admin` / `arsenal` | `172.0.0.1` |
| **🌐 WebUI Bruce de base** | `BruceNet` | `brucenet` | `admin` / `bruce` | `172.0.0.1` |

Tout est configurable depuis **Paramètres** sur l'appareil. Si un défaut ne marche pas, quelqu'un l'a changé — reflashez ou vérifiez Paramètres → WiFi AP / WebUI. L'IP est **`172.0.0.1`**, pas `192.168.4.1`.

## 🧰 Outils (68)

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

✅ = fonctionne sur Full et LITE. La plupart des Full-only nécessitent une carte 16 MB.

## 🖥️ Cartes

**Full (16 MB) :** T-Embed CC1101 · T-Deck / T-Deck Pro · T-Display S3 (all variants) · T-HMI · T-LoRa Pager · T-Watch S3 · M5Stack Cardputer / CoreS3 / StickS3 · ESP32-S3 DevKitC-1 · Smoochiee

**Full (4 MB) :** ESP32-C5 / ESP32-C5 TFT

**LITE (4 MB) :** M5Stack Core 4/16 MB · CPlus 1.1 · CYD-2432S028/2USB/W328C/W328R · CYD-3248S035R/C · Elecrow 2.4B/2.8B/3.5B · T-Display TTGO · Marauder Mini/V4-V6/V7/V6.1 · Awok Mini/Touch · WaveSentry R1 · Phantom S024R

**⚠️ Problèmes connus :** M5Stack CPlus2 & Core2 — DRAM overflow, use LITE.

## 🔨 Compiler depuis les sources

```bash
git clone https://github.com/quietdom/bruce-arsenal
cd bruce-arsenal
pio run -e <board-name>   # noms dans platformio.ini
```

## 👥 Crédits

- [Bruce Firmware](https://github.com/pr3y/Bruce) par pr3y — la base
- Module Arsenal + outils par quietdom

## ⚖️ Avertissement

Pour tests de sécurité autorisés et usages éducatifs uniquement. Ne soyez pas idiot avec ça.
