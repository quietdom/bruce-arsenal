?? [English](../README.md) | [Portugues](README_PT.md) | [Italiano](README_IT.md) | [Francais](README_FR.md) | [Russian](README_RU.md) | [Chinese](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Turkce](README_TR.md) | [Deutsch](README_DE.md) | [Vietnamese](README_VN.md) | [Espanol](README_ES.md) | [Indonesia](README_ID.md) | [Arabic](README_AR.md)

<h1 align="center">?? Bruce Arsenal</h1>

<p align="center">
  <b>Firmware de seguridad ofensiva personalizado para dispositivos ESP32</b><br>
  <i>72 herramientas - Un firmware - Multiples dispositivos</i>
</p>

---

## Que Es

Una version modificada de Bruce Firmware con un modulo Arsenal personalizado - 72 herramientas de seguridad ofensiva accesibles desde un solo menu.

---

## Instalacion

1. Descargue el .bin de los Releases
2. Flash via ESP Web Flasher (Chrome/Edge) o OTA via Arsenal Dashboard

---

## Herramientas Arsenal (72)

### WiFi Arsenal (11)
Network Scanner, DHCP Starvation, Karma Attack, DNS Spoofer, Auto-Phish Portal, Cred Forward, Auth Flood, AP Clone Flood, SSL Strip Lite, DNS Tunnel, WPS PIN Attack

### BLE Arsenal (6)
BLE Tracker, BT Name Spammer, AirTag Spoofer, Audio Jammer, Notification Spoofer, BT Rickroll

### Inteligencia (8)
Device Fingerprinter, OPSEC Monitor, OUI Lookup, Probe Log, Banner Grabber, SmartHome Scan, Channel Chart, People Counter

### Evasion (5)
MAC Rotator, Channel Hopper, Decoy Traffic, Identity Cloner, Time Randomizer

### Phishing (4)
Win Update Phish, WiFi Speed Phish, OAuth Phish, Device Found Phish

### Sub-GHz / RF (6)
NRF24 MouseJack, Doorbell Replay, Garage Brute Force, Keyfob Logger, Frequency Scanner, Flipper Import

### Deteccion (3)
Flipper Detector, Hacker Detector, RF Silence Enforcer

### Comunicaciones (5)
ESP-NOW Chat, ESP-NOW C2, Dead Drop Mesh, IR Data Transfer, Multi-Device Sync

### Utilidades (2)
NFC Biz Card, Attack Stats

---

## Compatibilidad de Placas

### ESP32-S3 (Completo - 72 herramientas)
LilyGo T-Embed CC1101, T-Deck Pro, T-Deck, T-Display S3, T-HMI, T-LoRa Pager, T-Watch S3, M5Stack Cardputer, CoreS3, StickS3

### ESP32 (LITE - 31 herramientas)
M5Stack Core 4MB/16MB, CPlus 1.1, CYD (all variants), Elecrow, Marauder, Awok, WaveSentry, Phantom

### Limitaciones Conocidas
M5Stack CPlus2, M5Stack Core2 - Desbordamiento DRAM (variables globales del firmware core)

---

## Servicios en Segundo Plano

- **OPSEC Dot** - Circulo colorido en la barra de estado
- **Always-On Evasion** - Rotacion MAC + salto de canales + trafico de distraccion
- **Auto-Dim** - Brillo de pantalla disminuye durante ataques

---

## Panel Remoto

Arsenal > Dashboard. Conecte el telefono a ArsenalNet (contrasena: arsenal32). Abra 192.168.4.1.

---

## Creditos

- [Bruce Firmware](https://github.com/pr3y/Bruce) by pr3y
- Arsenal module by quietdom

---

## Solo para pruebas de seguridad autorizadas y fines educativos.