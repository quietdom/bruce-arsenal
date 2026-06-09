?? [English](../README.md) | [Portugues](README_PT.md) | [Italiano](README_IT.md) | [Francais](README_FR.md) | [Russian](README_RU.md) | [Chinese](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Turkce](README_TR.md) | [Deutsch](README_DE.md) | [Vietnamese](README_VN.md) | [Espanol](README_ES.md) | [Indonesia](README_ID.md) | [Arabic](README_AR.md)

<h1 align="center">?? Bruce Arsenal</h1>

<p align="center">
  <b>Benutzerdefinierte Offensiv-Sicherheits-Firmware fur ESP32-Gerate</b><br>
  <i>72 Werkzeuge - Eine Firmware - Mehrere Gerate</i>
</p>

---

## Was Ist Das

Eine modifizierte Bruce Firmware mit benutzerdefiniertem Arsenal-Modul - 72 Offensiv-Sicherheitswerkzeuge aus einem einzigen Menu.

---

## Installation

1. Laden Sie die .bin aus den Releases herunter
2. Flashen Sie uber ESP Web Flasher (Chrome/Edge) oder OTA via Arsenal Dashboard

---

## Arsenal Werkzeuge (72)

### WiFi Arsenal (11)
Network Scanner, DHCP Starvation, Karma Attack, DNS Spoofer, Auto-Phish Portal, Cred Forward, Auth Flood, AP Clone Flood, SSL Strip Lite, DNS Tunnel, WPS PIN Attack

### BLE Arsenal (6)
BLE Tracker, BT Name Spammer, AirTag Spoofer, Audio Jammer, Notification Spoofer, BT Rickroll

### Intelligenz (8)
Device Fingerprinter, OPSEC Monitor, OUI Lookup, Probe Log, Banner Grabber, SmartHome Scan, Channel Chart, People Counter

### Umgehung (5)
MAC Rotator, Channel Hopper, Decoy Traffic, Identity Cloner, Time Randomizer

### Phishing (4)
Win Update Phish, WiFi Speed Phish, OAuth Phish, Device Found Phish

### Sub-GHz / RF (6)
NRF24 MouseJack, Doorbell Replay, Garage Brute Force, Keyfob Logger, Frequency Scanner, Flipper Import

### Erkennung (3)
Flipper Detector, Hacker Detector, RF Silence Enforcer

### Kommunikation (5)
ESP-NOW Chat, ESP-NOW C2, Dead Drop Mesh, IR Data Transfer, Multi-Device Sync

### Dienstprogramme (2)
NFC Biz Card, Attack Stats

---

## Board-Kompatibilitat

### ESP32-S3 (Vollstandig - 72 Werkzeuge)
LilyGo T-Embed CC1101, T-Deck Pro, T-Deck, T-Display S3, T-HMI, T-LoRa Pager, T-Watch S3, M5Stack Cardputer, CoreS3, StickS3

### ESP32 (LITE - 31 Werkzeuge)
M5Stack Core 4MB/16MB, CPlus 1.1, CYD (all variants), Elecrow, Marauder, Awok, WaveSentry, Phantom

### Bekannte Einschrankungen
M5Stack CPlus2, M5Stack Core2 - DRAM-Uberlauf (Kern-Firmware-Globale)

---

## Hintergrunddienste

- **OPSEC Dot** - Farbiger Kreis in der Statusleiste
- **Always-On Evasion** - MAC-Rotation + Kanalsprung + Ablenkungsverkehr
- **Auto-Dim** - Bildschirmhelligkeit sinkt wahrend Angriffen

---

## Remote-Dashboard

Arsenal > Dashboard. Verbinden Sie Ihr Telefon mit ArsenalNet (Passwort: arsenal32). Offnen Sie 192.168.4.1.

---

## Danksagungen

- [Bruce Firmware](https://github.com/pr3y/Bruce) by pr3y
- Arsenal module by quietdom

---

## Nur fur autorisierte Sicherheitstests und Bildungszwecke.