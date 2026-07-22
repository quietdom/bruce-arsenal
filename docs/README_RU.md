🌐 [English](../README.md) | [Portugues](README_PT.md) | [Italiano](README_IT.md) | [Francais](README_FR.md) | **Русский** | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Turkce](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Espanol](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/artworks/bruce-working.jpg" alt="Bruce Arsenal" width="500">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>Кастомная атакующая прошивка для ESP32</b><br>
  <i>Одна прошивка, много плат — 68 инструментов</i>
</p>

---

Модифицированная [Bruce Firmware](https://github.com/pr3y/Bruce) с собственным модулем **Arsenal**. Все оригинальные функции Bruce сохранены. Компилируется и тестируется на 30 платах перед каждым релизом.

## Новое в v1.2.0

**Исправлено** — Зависание RF Listen на T-Embed (разделяемая шина SPI освобождается при выходе) · «CC1101 не найден» · NRF24 «едва работает» (проверяет чип и питание) · jam_all на 4 диапазонах · реальные счётчики отправлено/ошибки · ключ WireGuard больше не в serial · линкер deauther.

**Новое** — Universal IR Remote (по бренду, коды во flash, без SD) · PMKID Capture (WPA без клиента) · Deauth Detector (защитный) · Rolljam (jam+перехват keyfob) · настоящий MouseJack · сохранение/загрузка целей deauth · обновлённый BLE-спаммер (40 FastPair ID + 18 моделей Apple).

## Прошивка

1. Скачайте `.bin` для вашей платы из [Releases](../../releases)
2. Откройте [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
3. **Connect** → выберите порт ESP32 → **Program** → выберите `.bin` → адрес `0x0`
4. Нажмите **RST** или переподключите USB

OTA: подключитесь к AP (ниже), откройте панель, **OTA Update** → выберите новый `.bin`.

CLI: `esptool.py --port COM3 --baud 460800 write_flash 0x0 firmware.bin`

## Подключение к панели

Два набора учётных данных — зависят от запущенной функции:

| | AP name | AP password | Dashboard login | IP |
|---|---------|-------------|-----------------|----|
| **Панель Arsenal** | `ArsenalNet` | `arsenal32` | `admin` / `arsenal` | `172.0.0.1` |
| **Базовый Bruce WebUI** | `BruceNet` | `brucenet` | `admin` / `bruce` | `172.0.0.1` |

Всё настраивается в **Настройках** на устройстве. Если значение по умолчанию не работает, кто-то его изменил — перепрошейте или проверьте Настройки → WiFi AP / WebUI. IP — **`172.0.0.1`**, не `192.168.4.1`.

## Инструменты (68)

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

✅ = работает на Full и LITE. Большинство Full-only требуют плату 16 МБ.

## Платы

**Full (16 МБ):** T-Embed CC1101 · T-Deck / T-Deck Pro · T-Display S3 (all variants) · T-HMI · T-LoRa Pager · T-Watch S3 · M5Stack Cardputer / CoreS3 / StickS3 · ESP32-S3 DevKitC-1 · Smoochiee

**Full (4 МБ):** ESP32-C5 / ESP32-C5 TFT

**LITE (4 МБ):** M5Stack Core 4/16 MB · CPlus 1.1 · CYD-2432S028/2USB/W328C/W328R · CYD-3248S035R/C · Elecrow 2.4B/2.8B/3.5B · T-Display TTGO · Marauder Mini/V4-V6/V7/V6.1 · Awok Mini/Touch · WaveSentry R1 · Phantom S024R

**Известные проблемы:** M5Stack CPlus2 & Core2 — DRAM overflow, use LITE.

## Сборка из исходников

```bash
git clone https://github.com/quietdom/bruce-arsenal
cd bruce-arsenal
pio run -e <board-name>   # имена в platformio.ini
```

## Благодарности

- [Bruce Firmware](https://github.com/pr3y/Bruce) от pr3y — основа
- Модуль Arsenal + инструменты от quietdom

## Дисклеймер

Только для авторизованного тестирования безопасности и образовательных целей. Не будьте идиотом с этим.
