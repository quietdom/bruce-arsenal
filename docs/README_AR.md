🌐 [English](../README.md) | [Portugues](README_PT.md) | [Italiano](README_IT.md) | [Francais](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Turkce](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Espanol](README_ES.md) | [Indonesia](README_ID.md) | **العربية**

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>برمجيات أمنية هجومية مخصصة لأجهزة ESP32</b><br>
  <i>68 أداة — برمجة واحدة، أجهزة متعددة</i>
</p>

---

نسخة معدلة من [Bruce Firmware](https://github.com/pr3y/Bruce) مع وحدة **Arsenal** مخصصة. جميع ميزات Bruce الأصلية تبقى كما هي. تُجمَّع وتُختبر على 30 لوحة قبل كل إصدار.

## الجديد في v1.2.0

**تم الإصلاح:**
- تجمد T-Embed RF Listen — تم الإصلاح (تحرير ناقل SPI المشترك عند الخروج)
- "CC1101 غير موجود" — تم الإصلاح
- NRF24 "بالكاد يعمل" — تم الإصلاح (يتحقق من الشريحة ويغذيها)
- jam_all — تم إصلاح جميع النطاقات الأربعة
- عدادات flood/spam — تعرض الآن المرسل/الفاشل الحقيقي
- WireGuard — المفتاح الخاص لم يعد يُطبع على المنفذ التسلسلي
- Deauther — العودة إلى .cpp، تم إصلاح الربط

**جديد:**
- **Universal IR Remote** — جهاز تحكم عن بعد للتلفاز حسب العلامة، الأكواد في الذاكرة، بدون SD
- **PMKID Capture** — WPA بدون عميل، يحفظ HC22000 على SD
- **Deauth Detector** — دفاعي، ينبه عند إزالة مصادقتك
- **Rolljam** — يلتقط أكواد عبر تشويش + RX
- **Real MouseJack** — الآن يشغل التطبيق الحقيقي البالغ 934 سطرًا
- **Saved target lists** — يحفظ/يحمّل أهداف deauth على SD
- **Refreshed BLE spammer** — 40 معرّف FastPair حقيقي + 18 طراز Apple

## التثبيت

1. نزّل ملف `.bin` للوحة الخاصة بك من [Releases](../../releases)
2. افتح [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome أو Edge)
3. **Connect**، اخذ المنفذ التسلسلي لـ ESP32
4. **Program**، اختر `.bin`، العنوان `0x0`
5. برمج، انتظر، اضغط **RST** أو أعد توصيل USB

---

## أدوات Arsenal (68)

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

## توافق اللوحات

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
| **مشاكل معروفة** | | |
| M5Stack CPlus2 | — | DRAM overflow — use LITE |
| M5Stack Core2 | — | DRAM overflow — use LITE |

---

## لوحة التحكم عن بعد

Arsenal > Dashboard. اتصل بـ `BruceNet` (كلمة المرور: `bruce32`). افتح `192.168.4.1`.

## الاعتمادات

- [Bruce Firmware](https://github.com/pr3y/Bruce) بواسطة pr3y — أساس هذا المشروع
- وحدة Arsenal والأدوات المخصصة بواسطة quietdom

## إخلاء المسؤولية

لاستخدام اختبارات الأمان المصرح بها والأغراض التعليمية فقط. لا تكن غبيًا مع هذا.
