🌐 [English](../README.md) | [Portugues](README_PT.md) | [Italiano](README_IT.md) | [Francais](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Turkce](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Espanol](README_ES.md) | [Indonesia](README_ID.md) | **العربية**

<p align="center">
  <img src="../media/artworks/bruce-working.jpg" alt="Bruce Arsenal" width="500">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>برمجيات أمنية هجومية مخصصة لأجهزة ESP32</b><br>
  <i>برمجة واحدة، أجهزة متعددة — 68 أداة</i>
</p>

---

نسخة معدلة من [Bruce Firmware](https://github.com/pr3y/Bruce) مع وحدة **Arsenal** مخصصة. جميع ميزات Bruce الأصلية تبقى كما هي. تُجمَّع وتُختبر على 30 لوحة قبل كل إصدار.

## الجديد في v1.2.0

**تم الإصلاح** — تجمد T-Embed RF Listen (تحرير ناقل SPI المشترك عند الخروج)·"CC1101 غير موجود"·NRF24"بالكاد يعمل"(يتحقق من الشريحة ويغذيها)·jam_all على 4 نطاقات·عدادات المرسل/الفاشل الحقيقية·مفتاح WireGuard لم يعد على المنفذ التسلسلي·رابط deauther.

**جديد** — Universal IR Remote (حسب العلامة، الأكواد في الذاكرة، بدون SD)·PMKID Capture (WPA بدون عميل)·Deauth Detector (دفاعي)·Rolljam (تشويش+التقاط keyfob)·MouseJack حقيقي·حفظ/تحميل أهداف deauth·مرسل BLE محدّث (40 معرّف FastPair + 18 طراز Apple).

## التثبيت

1. نزّل ملف `.bin` للوحة الخاصة بك من [Releases](../../releases)
2. افتح [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
3. **Connect** → اختر منفذ ESP32 → **Program** → اختر `.bin` → العنوان `0x0`
4. اضغط **RST** أو أعد توصيل USB

OTA: اتصل بـ AP (بالأسفل)، افتح لوحة التحكم، **OTA Update** → اختر `.bin` الجديد.

CLI: `esptool.py --port COM3 --baud 460800 write_flash 0x0 firmware.bin`

## الاتصال بلوحة التحكم

هناك مجموعتان من بيانات الاعتماد — تعتمدان على الميزة التي فتحتها:

| | AP name | AP password | Dashboard login | IP |
|---|---------|-------------|-----------------|----|
| **لوحة Arsenal** | `ArsenalNet` | `arsenal32` | `admin` / `arsenal` | `172.0.0.1` |
| **واجهة Bruce الأساسية** | `BruceNet` | `brucenet` | `admin` / `bruce` | `172.0.0.1` |

كل شيء قابل للتكوين من **الإعدادات** على الجهاز. إذا لم يعمل الإعداد الافتراضي، فقد غيّره شخص ما — أعد التثبيت أو تحقق من الإعدادات → WiFi AP / WebUI. عنوان IP هو **`172.0.0.1`**، وليس `192.168.4.1`.

## الأدوات (68)

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

✅ = يعمل على Full و LITE. معظم أدوات Full-only تتطلب لوحة 16 ميغابايت.

## اللوحات

**Full (16 MB):** T-Embed CC1101 · T-Deck / T-Deck Pro · T-Display S3 (all variants) · T-HMI · T-LoRa Pager · T-Watch S3 · M5Stack Cardputer / CoreS3 / StickS3 · ESP32-S3 DevKitC-1 · Smoochiee

**Full (4 MB):** ESP32-C5 / ESP32-C5 TFT

**LITE (4 MB):** M5Stack Core 4/16 MB · CPlus 1.1 · CYD-2432S028/2USB/W328C/W328R · CYD-3248S035R/C · Elecrow 2.4B/2.8B/3.5B · T-Display TTGO · Marauder Mini/V4-V6/V7/V6.1 · Awok Mini/Touch · WaveSentry R1 · Phantom S024R

**مشاكل معروفة:** M5Stack CPlus2 & Core2 — DRAM overflow, use LITE.

## التجميع من المصدر

```bash
git clone https://github.com/quietdom/bruce-arsenal
cd bruce-arsenal
pio run -e <board-name>   # الأسماء في platformio.ini
```

## الاعتمادات

- [Bruce Firmware](https://github.com/pr3y/Bruce) بواسطة pr3y — الأساس
- وحدة Arsenal + الأدوات بواسطة quietdom

## إخلاء المسؤولية

لاستخدام اختبارات الأمان المصرح بها والأغراض التعليمية فقط. لا تكن غبيًا مع هذا.
