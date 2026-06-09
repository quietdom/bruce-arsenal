?? [English](../README.md) | [Portugues](README_PT.md) | [Italiano](README_IT.md) | [Francais](README_FR.md) | [Russian](README_RU.md) | [Chinese](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Turkce](README_TR.md) | [Deutsch](README_DE.md) | [Vietnamese](README_VN.md) | [Espanol](README_ES.md) | [Indonesia](README_ID.md) | [Arabic](README_AR.md)

<h1 align="center">?? Bruce Arsenal</h1>

<p align="center">
  <b>Niestandardowe firmware bezpieczenstwa ofensywnego dla urzadzen ESP32</b><br>
  <i>72 narzedzia - Jeden firmware - Wiele urzadzen</i>
</p>

---

## Co To Jest

Zmodyfikowana Bruce Firmware z niestandardowym模ulem Arsenal - 72 narzedzia bezpieczenstwa ofensywnego dostepne z jednego menu.

---

## Instalacja

1. Pobierz .bin z Releases
2. Flash przez ESP Web Flasher (Chrome/Edge) lub OTA przez Arsenal Dashboard

---

## Narzedzia Arsenal (72)

### WiFi Arsenal (11)
Network Scanner, DHCP Starvation, Karma Attack, DNS Spoofer, Auto-Phish Portal, Cred Forward, Auth Flood, AP Clone Flood, SSL Strip Lite, DNS Tunnel, WPS PIN Attack

### BLE Arsenal (6)
BLE Tracker, BT Name Spammer, AirTag Spoofer, Audio Jammer, Notification Spoofer, BT Rickroll

### Wywiad (8)
Device Fingerprinter, OPSEC Monitor, OUI Lookup, Probe Log, Banner Grabber, SmartHome Scan, Channel Chart, People Counter

### Unikanie (5)
MAC Rotator, Channel Hopper, Decoy Traffic, Identity Cloner, Time Randomizer

### Phishing (4)
Win Update Phish, WiFi Speed Phish, OAuth Phish, Device Found Phish

### Sub-GHz / RF (6)
NRF24 MouseJack, Doorbell Replay, Garage Brute Force, Keyfob Logger, Frequency Scanner, Flipper Import

### Wykrywanie (3)
Flipper Detector, Hacker Detector, RF Silence Enforcer

### Komunikacja (5)
ESP-NOW Chat, ESP-NOW C2, Dead Drop Mesh, IR Data Transfer, Multi-Device Sync

### Narzedzia (2)
NFC Biz Card, Attack Stats

---

## Kompatybilnosc Plyt

### ESP32-S3 (Pelny - 72 narzedzia)
LilyGo T-Embed CC1101, T-Deck Pro, T-Deck, T-Display S3, T-HMI, T-LoRa Pager, T-Watch S3, M5Stack Cardputer, CoreS3, StickS3

### ESP32 (LITE - 31 narzedzia)
M5Stack Core 4MB/16MB, CPlus 1.1, CYD (all variants), Elecrow, Marauder, Awok, WaveSentry, Phantom

### Znane Ograniczenia
M5Stack CPlus2, M5Stack Core2 - Przepelnienie DRAM (zmienne globalne firmware)

---

## Uslugi Wtle

- **OPSEC Dot** - Kolorowy okrag na pasku statusu
- **Always-On Evasion** - Rotacja MAC + przeskakiwanie kanalow + ruch odwrocujacy
- **Auto-Dim** - Jasnosc ekranu spada podczas atakow

---

## Panel Zdalny

Arsenal > Dashboard. Polacz telefon z ArsenalNet (haslo: arsenal32). Otworz 192.168.4.1.

---

## Autorzy

- [Bruce Firmware](https://github.com/pr3y/Bruce) by pr3y
- Arsenal module by quietdom

---

## Tylko do autoryzowanych testow bezpieczenstwa i celow edukacyjnych.