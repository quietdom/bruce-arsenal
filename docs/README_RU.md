🌐 [English](../README.md) | [Portugues](README_PT.md) | [Italiano](README_IT.md) | [Francais](README_FR.md) | **Русский** | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Turkce](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Espanol](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>Кастомная атакующая прошивка для устройств ESP32</b><br>
  <i>68 инструментов — одна прошивка, много плат</i>
</p>

---

Модифицированная [Bruce Firmware](https://github.com/pr3y/Bruce) с собственным модулем **Arsenal**. Все оригинальные функции Bruce сохранены. Компилируется и тестируется на 30 платах перед каждым релизом.

## Новое в v1.2.0

**Исправлено:**
- Зависание RF Listen на T-Embed — исправлено (разделяемая шина SPI освобождается при выходе)
- «CC1101 не найден» — исправлено
- NRF24 «едва работает» — исправлено (проверяет чип и питание)
- jam_all — исправлены все 4 диапазона
- Счётчики flood/spam — теперь показывают реальные отправлено/ошибки
- WireGuard — приватный ключ больше не выводится в serial
- Deauther — реализация обратно в .cpp, линковка исправлена

**Новое:**
- **Universal IR Remote** — ТВ-пульт по бренду, коды во flash, без SD
- **PMKID Capture** — WPA без клиента, сохраняет HC22000 на SD
- **Deauth Detector** — защитный, предупреждает когда вас деаутентифицируют
- **Rolljam** — перехватывает коды через jam + RX
- **Real MouseJack** — теперь запускает настоящую 934-строчную реализацию
- **Saved target lists** — сохраняет/загружает цели deauth на SD
- **Refreshed BLE spammer** — 40 реальных FastPair ID + 18 моделей Apple

## Установка

1. Скачайте `.bin` для вашей платы из [Releases](../../releases)
2. Откройте [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome или Edge)
3. **Connect**, выберите последовательный порт ESP32
4. **Program**, выберите `.bin`, адрес `0x0`
5. Прошейте, подождите, нажмите **RST** или переподключите USB

---

## Инструменты Arsenal (68)

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

## Совместимость плат

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
| **Известные проблемы** | | |
| M5Stack CPlus2 | — | DRAM overflow — use LITE |
| M5Stack Core2 | — | DRAM overflow — use LITE |

---

## Удалённая панель

Arsenal > Dashboard. Подключитесь к `BruceNet` (пароль: `bruce32`). Откройте `192.168.4.1`.

## Благодарности

- [Bruce Firmware](https://github.com/pr3y/Bruce) от pr3y — основа этого проекта
- Модуль Arsenal и кастомные инструменты — quietdom

## Дисклеймер

Только для авторизованного тестирования безопасности и образовательных целей. Не будьте идиотом с этим.
