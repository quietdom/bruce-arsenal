?? [English](../README.md) | [Portugues](README_PT.md) | [Italiano](README_IT.md) | [Francais](README_FR.md) | [Russian](README_RU.md) | [Chinese](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Turkce](README_TR.md) | [Deutsch](README_DE.md) | [Vietnamese](README_VN.md) | [Espanol](README_ES.md) | [Indonesia](README_ID.md) | [Arabic](README_AR.md)

<h1 align="center">?? Bruce Arsenal</h1>

<p align="center">
  <b>Пользовательская прошивка наступательной безопасности для устройств ESP32</b><br>
  <i>72 инструмента - Одна прошивка - Несколько устройств</i>
</p>

---

## Что Это

Модифицированная Bruce Firmware с пользовательским модуlem Arsenal - 72 инструмента наступательной безопасности из одного меню.

---

## Установка

1. Скачайте .bin из Releases
2. Прошейте через ESP Web Flasher (Chrome/Edge) или OTA через Arsenal Dashboard

---

## Инструменты Arsenal (72)

### WiFi Арсенал (11)
Network Scanner, DHCP Starvation, Karma Attack, DNS Spoofer, Auto-Phish Portal, Cred Forward, Auth Flood, AP Clone Flood, SSL Strip Lite, DNS Tunnel, WPS PIN Attack

### BLE Арсенал (6)
BLE Tracker, BT Name Spammer, AirTag Spoofer, Audio Jammer, Notification Spoofer, BT Rickroll

### Разведка (8)
Device Fingerprinter, OPSEC Monitor, OUI Lookup, Probe Log, Banner Grabber, SmartHome Scan, Channel Chart, People Counter

### Уклонение (5)
MAC Rotator, Channel Hopper, Decoy Traffic, Identity Cloner, Time Randomizer

### Фишинг (4)
Win Update Phish, WiFi Speed Phish, OAuth Phish, Device Found Phish

### Sub-GHz / RF (6)
NRF24 MouseJack, Doorbell Replay, Garage Brute Force, Keyfob Logger, Frequency Scanner, Flipper Import

### Обнаружение (3)
Flipper Detector, Hacker Detector, RF Silence Enforcer

### Связь (5)
ESP-NOW Chat, ESP-NOW C2, Dead Drop Mesh, IR Data Transfer, Multi-Device Sync

### Утилиты (2)
NFC Biz Card, Attack Stats

---

## Совместимость плат

### ESP32-S3 (Полный - 72 инструментов)
LilyGo T-Embed CC1101, T-Deck Pro, T-Deck, T-Display S3, T-HMI, T-LoRa Pager, T-Watch S3, M5Stack Cardputer, CoreS3, StickS3

### ESP32 (LITE - 31 инструментов)
M5Stack Core 4MB/16MB, CPlus 1.1, CYD (all variants), Elecrow, Marauder, Awok, WaveSentry, Phantom

### Известные ограничения
M5Stack CPlus2, M5Stack Core2 - Переполнение DRAM (глобальные переменные ядра)

---

## Фоновые службы

- **OPSEC Dot** - Цветной круг на панели состояния
- **Always-On Evasion** - Ротация MAC + переключение каналов + отвлекающий трафик
- **Auto-Dim** - Яркость экрана снижается во время атак

---

## Панель управления

Arsenal > Dashboard. Подключите телефон к ArsenalNet (пароль: arsenal32). Откройте 192.168.4.1.

---

## Авторы

- [Bruce Firmware](https://github.com/pr3y/Bruce) by pr3y
- Arsenal module by quietdom

---

## Только для авторизованного тестирования безопасности и образовательных целей.