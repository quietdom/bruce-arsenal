🌐 [English](../README.md) | [Português](README_PT.md) | [Italiano](README_IT.md) | [Français](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Türkçe](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Español](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>ESP32 cihazlar için özel saldırı güvenlik yazılımı</b><br>
  <i>26 saldırı aracı • Mobil kontrol paneli • Arka plan kaçınma • 60K+ betik</i>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32--S3-blue?style=flat-square" alt="ESP32-S3">
  <img src="https://img.shields.io/badge/Target-T--Embed%20CC1101-green?style=flat-square" alt="T-Embed">
  <img src="https://img.shields.io/badge/Tools-26+-red?style=flat-square" alt="Tools">
  <img src="https://img.shields.io/badge/Scripts-60K+-purple?style=flat-square" alt="Scripts">
  <img src="https://img.shields.io/badge/Status-Active-brightgreen?style=flat-square" alt="Status">
</p>

---

## Bu Nedir

[Bruce Firmware](https://github.com/pr3y/Bruce)'ın kapsamlı bir şekilde değiştirilmiş versiyonu, entegre edilmiş özel bir **Arsenal** modülü ile — nişangah simgeli tek bir menü sekmesinden erişilebilen tam bir saldırı güvenlik araç seti.

Tüm orijinal Bruce özellikleri bozulmadan kalır. Arsenal 26 yeni araç, mobil web kontrol paneli, arka plan kaçınma servisleri ve SD karttan 60K+ payload yükleyebilen bir betik tarayıcısı ekler.

---

## Kurulum

1. `.bin` dosyasını [Releases](../../releases) sayfasından indirin
2. Cihazınıza şu yöntemlerden biriyle yükleyin:
   - **USB:** [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
   - **OTA:** İlk yüklemeden sonra Arsenal → Dashboard → telefondan yeni .bin yükleyin
3. Tamam

---

## Arsenal Menüsü

```
🎯 Arsenal
├── WiFi Arsenal ──────── 8 saldırı WiFi aracı
├── BLE Arsenal ───────── 5 Bluetooth saldırı aracı
├── İstihbarat ────────── 3 keşif/farkındalık aracı
├── Kaçınma ───────────── 5 gizlilik/anti-algılama aracı
├── Kombolar ──────────── Önceden ayarlanmış + özel saldırı zincirleri
├── Zamanlayıcı ────────── Gecikmeli saldırı yürütme
├── Betikler ──────────── SD'den 60K+ payload göz atma
├── Oturum Günlüğü ────── SD'ye aktivite kaydı
├── Jam All ───────────── Çoklu bant eşzamanlı karıştırma (özel GUI)
└── Dashboard ─────────── Mobil web UI + dosya yöneticisi + OTA
```

---

## Araçlar

### WiFi Arsenal

| Araç | Ne Yapar |
|------|----------|
| **Network Scanner** | ARP tarama + TCP port tarama. Cihazları, açık portları, üretici adlarını gösterir |
| **DHCP Starvation** | Sahte MAC'lerle DHCP isteklerini doldurup yönlendiricinin IP havuzunu tüketir |
| **Karma Attack** | Tüm WiFi probe isteklerine yanıt verir — yakındaki cihazlar otomatik olarak size bağlanır |
| **DNS Spoofer** | AP olarak çalışırken DNS'yi yakalar. Tüm alan adları portalınıza yönlenir |
| **Portal Templates** | SD karttan özel phishing HTML yükler. Kimlik bilgilerini dosyaya kaydeder |
| **Auto-Phish Portal** | Kurbanların hangi ağları aradığını tespit eder, SSID'yi klonlar, portal sunar |
| **Cred Forward** | Portal kimlik bilgilerini yakalar → gerçek AP'ye otomatik bağlanır → trafiği köprüler. Tam şeffaf MITM |
| **WiFi Brute Force** | SSID adından akıllı kelime listesi üretir (yıl kombinasyonları, leet speak, yaygın kalıplar). Canlı olarak dener |

### BLE Arsenal

| Araç | Ne Yapar |
|------|----------|
| **BLE Tracker** | RSSI mesafe tahmini ile sürekli BLE tarama. Sizi takip eden AirTag, SmartTag, Tile'ları tespit eder |
| **Name Spammer** | BLE advertising kanallarını yüzlerce rastgele cihaz adıyla doldurur |
| **AirTag Spoofer** | Sahte Apple FindMy AirTag beacon'ları yayınlar. Hayalet etiketler yakındaki iPhone'larda görünür |
| **Audio Jammer** | BLE ses bağlantılarını (AirPods, hoparlörler) advertisement taşkını ile bozar |
| **Notification Spoofer** | Android (Fast Pair), Windows (Swift Pair) ve iOS (AirPods yakınlık) cihazlarına sahte cihaz bulundu bildirimleri gönderir |

### İstihbarat

| Araç | Ne Yapar |
|------|----------|
| **Device Fingerprinter** | Pasif WiFi parmak izi alma. Probe davranışı, OUI, zamanlama ile cihaz OS/modelini belirler |
| **OPSEC Monitor** | Taranıp taranmadığınızı, deauth yapılıp yapılmadığını veya izlenip izlenmediğinizi tespit eder. Tehdit seviyesi: yeşil/sarı/kırmızı |
| **OUI Lookup** | SD karttaki MAC üretici veritabanından çevrimdışı arama |

### Kaçınma

| Araç | Ne Yapar |
|------|----------|
| **MAC Rotator** | WiFi MAC adresini yapılandırılabilir zamanlayıcıda rastgele değiştirir. Ayarlanabilir hız |
| **Channel Hopper** | Operasyonlar sırasında WiFi kanallarını hızla değiştirir. Yapılandırılabilir aralık |
| **Decoy Traffic** | Dönen kanallarda rastgele beacon/probe çerçeveleri yayar. Gerçek aktiviteyi gürültüde maskeler |
| **Identity Cloner** | Yakındaki cihaz kimliklerini yakalar (MAC + probe kalıbı). Birini klonlayarak görünmez olun |
| **QR Poisoner** | Portalınıza yönlenen QR kodları üretir. Ekranda gösterin, yazdırın ve yerleştirin |

### Jam All (Özel GUI)

Bant bazında geçişler, canlı aktivite çubukları, geçen süre zamanlayıcısı ve tek tuşla durdurma ile tam ekran kontrol paneli.

| Bant | Yöntem |
|------|--------|
| WiFi 2.4GHz | Tüm 14 kanalda döngüsel deauth taşkını |
| BLE | Advertisement kanalı taşkını (37, 38, 39) |
| Sub-GHz | CC1101 sürekli TX (315/433/868/915 MHz) |
| NRF24 | Dönen kanallarda 2.4GHz taşıyıcı |

### Yardımcı Araçlar

| Araç | Ne Yapar |
|------|----------|
| **Attack Scheduler** | Bir araç seç → gecikme ayarla (0'dan 1 saate) → süre ayarla. Otomatik başlar ve durur |
| **Session Log** | Her eylemi zaman damgasıyla kaydeder. Menüden görüntüle/dışa aktar/temizle |
| **Script Browser** | Kategorize edilmiş SD klasörlerinde gezin. Dosya türüne göre otomatik başlatır (.txt/.sub/.ir/.html/.nfc/.js) |
| **Combo Presets** | Tek dokunuşla saldırı zincirleri. Yerleşik: Stealth Mode, Full Attack, Passive Recon. SD'de özel .txt oluşturun |
| **Remote Dashboard** | Telefon ArsenalNet AP'ye bağlanır → dosya yöneticisi + OTA güncellemeleri ile tam mobil web UI |

---

## Arka Plan Servisleri

Bunlar tüm menülerde kalıcı olarak çalışır — sadece Arsenal içinde değil.

| Servis | Açıklama |
|--------|----------|
| **OPSEC Dot** | Durum çubuğunda renkli daire (yeşil/sarı/kırmızı). Her zaman görünür, her ekranda |
| **Always-On Evasion** | MAC rotasyonu + channel hopping + decoy traffic. CPU çekirdeği 0'da sessizce çalışır |
| **Low Power Mode** | Pil tasarrufu için arka plan görevlerini 5x yavaşlatır. Combos menüsünden açılıp kapatılır |
| **Auto-Dim** | Aktif saldırılarda ekran parlaklığı düşer. Durduğunda geri yüklenir |

---

## Remote Dashboard

Arsenal → Dashboard başlatın. Telefonunuzu `ArsenalNet`'e bağlayın (şifre: `arsenal32`). Tarayıcıda `192.168.4.1` açın.

**Özellikler:**
- Telefondan tüm 26 aracı başlat/durdur
- SD karttaki dosyalara göz at, yükle, sil (`/arsenal/` klasörü)
- OTA güncellemesi için `.bin` firmware yükle
- Kategorize edilmiş klasörlere sürükle-bırak betik yükleme
- Canlı sistem durumu (heap, SD alanı, çalışma süresi)

---

## SD Kart Düzeni

```
/arsenal/
├── badusb/       DuckyScript payload'ları (.txt)
├── subghz/       Sub-GHz sinyal yakalamaları (.sub)
├── ir/           IR uzaktan kumanda dosyaları (.ir)
├── portals/      Evil portal HTML şablonları
├── nfc/          NFC kart dökümleri (.nfc)
├── rfid/         125kHz RFID yakalamaları (.rfid)
├── ibutton/      iButton anahtar dosyaları (.ibutton)
├── scripts/      JavaScript otomasyonu (.js)
├── combos/       Özel kombo ön ayarları (.txt — satır başına bir özellik adı)
├── logs/         Oturum günlükleri (otomatik oluşturulur)
└── creds.txt     Yakalanan kimlik bilgileri (otomatik oluşturulur)
```

[BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts) (60K+ betik) ile uyumlu.

---

## Özel Kombolar

`/arsenal/combos/` içinde satır başına bir özellik ile `.txt` dosyası oluşturun:

```
mac_rotator
channel_hopper
decoy_traffic
ble_tracker
```

Arsenal → Combos'da otomatik olarak görünür.

---

## Donanım

Birincil hedef: **LilyGo T-Embed CC1101**
- ESP32-S3 (16MB flash, 8MB PSRAM)
- ST7789 320x170 TFT ekran
- Döner enkoder + düğme
- CC1101 Sub-GHz radyo
- NRF24 2.4GHz radyo
- PN532 NFC
- SD kart yuvası
- USB-C (BadUSB HID)
- Hoparlör + Mikrofon
- RGB LED'ler
- BQ27220 yakıt ölçer

---

## Teşekkürler

- pr3y tarafından oluşturulan [Bruce Firmware](https://github.com/pr3y/Bruce) üzerine kurulu
- Arsenal modülü quietdom tarafından
- Betik koleksiyonu [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts) ile uyumlu

---

## Sorumluluk Reddi

Bu yazılım yalnızca yetkili güvenlik testleri ve eğitim amaçlıdır. Sahip olmadığınız veya test etmek için açık iznin olmadığı ağlara veya cihazlara karşı kullanmayın. Yazarlar kötüye kullanımdan sorumlu değildir.
