<div dir="rtl">

🌐 [English](../README.md) | [Português](README_PT.md) | [Italiano](README_IT.md) | [Français](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Türkçe](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Español](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>برنامج ثابت هجومي مخصص للأمن السيبراني لأجهزة ESP32</b><br>
  <i>26 أداة هجوم • لوحة تحكم متنقلة • تهرب في الخلفية • 60K+ نص برمجي</i>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32--S3-blue?style=flat-square" alt="ESP32-S3">
  <img src="https://img.shields.io/badge/Target-T--Embed%20CC1101-green?style=flat-square" alt="T-Embed">
  <img src="https://img.shields.io/badge/Tools-26+-red?style=flat-square" alt="Tools">
  <img src="https://img.shields.io/badge/Scripts-60K+-purple?style=flat-square" alt="Scripts">
  <img src="https://img.shields.io/badge/Status-Active-brightgreen?style=flat-square" alt="Status">
</p>

---

## ما هذا

نسخة معدلة بشكل كبير من [Bruce Firmware](https://github.com/pr3y/Bruce) مع وحدة **Arsenal** مخصصة مدمجة — مجموعة أدوات أمنية هجومية كاملة يمكن الوصول إليها من علامة تبويب قائمة واحدة بأيقونة التصويب.

جميع ميزات Bruce الأصلية تبقى سليمة. يضيف Arsenal 26 أداة جديدة، ولوحة تحكم ويب متنقلة، وخدمات تهرب في الخلفية، ومتصفح نصوص برمجية يمكنه تحميل أكثر من 60 ألف حمولة من بطاقة SD.

---

## التثبيت

1. قم بتنزيل ملف `.bin` من [Releases](../../releases)
2. قم بتحميله على جهازك باستخدام إحدى الطرق:
   - **USB:** [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
   - **OTA:** بعد التحميل الأول، استخدم Arsenal → Dashboard → ارفع ملف .bin جديد من الهاتف
3. تم

---

## قائمة Arsenal

```
🎯 Arsenal
├── WiFi Arsenal ──────── 8 أدوات WiFi هجومية
├── BLE Arsenal ───────── 5 أدوات هجوم Bluetooth
├── الاستخبارات ────────── 3 أدوات استطلاع/وعي
├── التهرب ─────────────── 5 أدوات تخفي/مضادة للكشف
├── التوليفات ──────────── سلاسل هجوم مسبقة + مخصصة
├── المجدول ────────────── تنفيذ هجوم مؤجل زمنياً
├── النصوص البرمجية ────── تصفح 60K+ حمولة من SD
├── سجل الجلسة ─────────── تسجيل النشاط على SD
├── Jam All ───────────── تشويش متزامن متعدد النطاقات (واجهة مخصصة)
└── Dashboard ─────────── واجهة ويب متنقلة + مدير ملفات + OTA
```

---

## الأدوات

### WiFi Arsenal

| الأداة | ما تفعله |
|--------|----------|
| **Network Scanner** | مسح ARP + فحص منافذ TCP. يعرض الأجهزة والمنافذ المفتوحة وأسماء المصنعين |
| **DHCP Starvation** | يغمر طلبات DHCP بعناوين MAC مزيفة لاستنفاد مجموعة IP الخاصة بالراوتر |
| **Karma Attack** | يستجيب لجميع طلبات WiFi probe — الأجهزة القريبة تتصل تلقائياً بك |
| **DNS Spoofer** | يعترض DNS عند التشغيل كـ AP. جميع النطاقات تحل إلى بوابتك |
| **Portal Templates** | يحمل HTML تصيد مخصص من بطاقة SD. يلتقط بيانات الاعتماد إلى ملف |
| **Auto-Phish Portal** | يكتشف الشبكات التي يبحث عنها الضحايا، يستنسخ SSID، يقدم البوابة |
| **Cred Forward** | يلتقط بيانات اعتماد البوابة → يتصل تلقائياً بـ AP الحقيقي → يجسر حركة المرور. MITM شفاف كامل |
| **WiFi Brute Force** | يولد قائمة كلمات ذكية من اسم SSID (توليفات سنوات، leet speak، أنماط شائعة). يجرب مباشرة |

### BLE Arsenal

| الأداة | ما تفعله |
|--------|----------|
| **BLE Tracker** | مسح BLE مستمر مع تقدير مسافة RSSI. يكشف AirTag وSmartTag وTile التي تتبعك |
| **Name Spammer** | يغمر قنوات BLE advertising بمئات أسماء الأجهزة العشوائية |
| **AirTag Spoofer** | يبث إشارات Apple FindMy AirTag مزيفة. علامات وهمية تظهر على أجهزة iPhone القريبة |
| **Audio Jammer** | يعطل اتصالات BLE الصوتية (AirPods، مكبرات الصوت) بطوفان advertisements |
| **Notification Spoofer** | يدفع نوافذ منبثقة مزيفة لجهاز تم العثور عليه إلى Android (Fast Pair) وWindows (Swift Pair) وiOS (قرب AirPods) |

### الاستخبارات

| الأداة | ما تفعله |
|--------|----------|
| **Device Fingerprinter** | بصمة WiFi سلبية. يحدد نظام تشغيل/طراز الجهاز من سلوك probe وOUI والتوقيت |
| **OPSEC Monitor** | يكشف إذا كنت تُفحص أو يُلغى مصادقتك أو تُتبع. مستوى التهديد: أخضر/أصفر/أحمر |
| **OUI Lookup** | بحث غير متصل في قاعدة بيانات مصنعي MAC من بطاقة SD |

### التهرب

| الأداة | ما تفعله |
|--------|----------|
| **MAC Rotator** | يعشوئ عنوان MAC WiFi على مؤقت قابل للتكوين. سرعة قابلة للتعديل |
| **Channel Hopper** | يبدل قنوات WiFi بسرعة أثناء العمليات. فاصل زمني قابل للتكوين |
| **Decoy Traffic** | يصدر إطارات beacon/probe عشوائية على قنوات دوارة. يخفي النشاط الحقيقي في الضوضاء |
| **Identity Cloner** | يلتقط هويات الأجهزة القريبة (MAC + نمط probe). استنسخ واحدة لتصبح غير مرئي |
| **QR Poisoner** | يولد رموز QR تشير إلى بوابتك. اعرض على الشاشة، اطبع وضع |

### Jam All (واجهة مخصصة)

لوحة تحكم بملء الشاشة مع مفاتيح لكل نطاق، أشرطة نشاط مباشرة، مؤقت منقضي وزر إيقاف فوري.

| النطاق | الطريقة |
|--------|---------|
| WiFi 2.4GHz | طوفان deauth يدور على جميع القنوات الـ 14 |
| BLE | طوفان قنوات advertisement (37، 38، 39) |
| Sub-GHz | إرسال CC1101 مستمر (315/433/868/915 MHz) |
| NRF24 | حامل 2.4GHz على قنوات دوارة |

### الأدوات المساعدة

| الأداة | ما تفعله |
|--------|----------|
| **Attack Scheduler** | اختر أداة → حدد التأخير (0 إلى ساعة واحدة) → حدد المدة. يبدأ ويتوقف تلقائياً |
| **Session Log** | يسجل كل إجراء مع طوابع زمنية. عرض/تصدير/مسح من القائمة |
| **Script Browser** | تصفح مجلدات SD المصنفة. يطلق تلقائياً حسب نوع الملف (.txt/.sub/.ir/.html/.nfc/.js) |
| **Combo Presets** | سلاسل هجوم بلمسة واحدة. مدمجة: Stealth Mode، Full Attack، Passive Recon. أنشئ مخصصة بملف .txt على SD |
| **Remote Dashboard** | يتصل الهاتف بـ AP ArsenalNet → واجهة ويب متنقلة كاملة مع مدير ملفات + تحديثات OTA |

---

## خدمات الخلفية

تعمل هذه بشكل مستمر عبر جميع القوائم — ليس فقط داخل Arsenal.

| الخدمة | الوصف |
|--------|-------|
| **OPSEC Dot** | دائرة ملونة في شريط الحالة (أخضر/أصفر/أحمر). مرئية دائماً، في كل شاشة |
| **Always-On Evasion** | تدوير MAC + channel hopping + decoy traffic. يعمل بصمت على نواة CPU 0 |
| **Low Power Mode** | يبطئ مهام الخلفية 5x لتوفير البطارية. التبديل من قائمة Combos |
| **Auto-Dim** | ينخفض سطوع الشاشة أثناء الهجمات النشطة. يستعيد عند التوقف |

---

## Remote Dashboard

ابدأ Arsenal → Dashboard. صل هاتفك بـ `ArsenalNet` (كلمة المرور: `arsenal32`). افتح `192.168.4.1` في المتصفح.

**الميزات:**
- بدء/إيقاف جميع الأدوات الـ 26 من الهاتف
- تصفح، رفع، حذف الملفات على بطاقة SD (مجلد `/arsenal/`)
- رفع ملف firmware `.bin` لتحديث OTA
- رفع النصوص البرمجية بالسحب والإفلات إلى مجلدات مصنفة
- حالة النظام مباشرة (heap، مساحة SD، وقت التشغيل)

---

## تخطيط بطاقة SD

```
/arsenal/
├── badusb/       حمولات DuckyScript (.txt)
├── subghz/       التقاطات إشارات Sub-GHz (.sub)
├── ir/           ملفات التحكم عن بعد IR (.ir)
├── portals/      قوالب HTML evil portal
├── nfc/          تفريغات بطاقات NFC (.nfc)
├── rfid/         التقاطات RFID 125kHz (.rfid)
├── ibutton/      ملفات مفاتيح iButton (.ibutton)
├── scripts/      أتمتة JavaScript (.js)
├── combos/       إعدادات combo مخصصة (.txt — اسم ميزة واحد لكل سطر)
├── logs/         سجلات الجلسة (يُنشأ تلقائياً)
└── creds.txt     بيانات الاعتماد الملتقطة (يُنشأ تلقائياً)
```

متوافق مع [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts) (60K+ نص برمجي).

---

## التوليفات المخصصة

أنشئ ملف `.txt` في `/arsenal/combos/` مع ميزة واحدة لكل سطر:

```
mac_rotator
channel_hopper
decoy_traffic
ble_tracker
```

يظهر تلقائياً في Arsenal → Combos.

---

## العتاد

الهدف الأساسي: **LilyGo T-Embed CC1101**
- ESP32-S3 (16MB flash، 8MB PSRAM)
- شاشة TFT ST7789 بحجم 320x170
- مشفر دوار + زر
- راديو Sub-GHz CC1101
- راديو NRF24 2.4GHz
- NFC PN532
- فتحة بطاقة SD
- USB-C (BadUSB HID)
- مكبر صوت + ميكروفون
- مصابيح LED RGB
- مقياس الوقود BQ27220

---

## الشكر والتقدير

- مبني على [Bruce Firmware](https://github.com/pr3y/Bruce) بواسطة pr3y
- وحدة Arsenal بواسطة quietdom
- مجموعة النصوص البرمجية متوافقة مع [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts)

---

## إخلاء المسؤولية

هذا البرنامج الثابت مخصص فقط لاختبار الأمان المصرح به والأغراض التعليمية. لا تستخدمه ضد شبكات أو أجهزة لا تملكها أو ليس لديك إذن صريح لاختبارها. المؤلفون غير مسؤولين عن سوء الاستخدام.

</div>
