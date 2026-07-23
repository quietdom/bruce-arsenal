🌐 [English](../README.md) | [Portugues](README_PT.md) | [Italiano](README_IT.md) | [Francais](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | **Turkce** | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Espanol](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>🔧 ESP32 için özel saldırı güvenlik donanım yazılımı</b><br>
  <i>⚡ Tek donanım yazılımı, birçok kart — 68 araç</i>
</p>

---

Özel bir **Arsenal** modülü ile değiştirilmiş bir [Bruce Firmware](https://github.com/pr3y/Bruce). Tüm orijinal Bruce özellikleri olduğu gibi kalır. Her sürümden önce 30 kartta derlenir ve test edilir.

## 🆕 v1.2.0'daki yenilikler

**🐛 Düzeltildi** — T-Embed RF Listen donması (paylaşılan SPI veriyolu çıkışta serbest bırakıldı) · "CC1101 bulunamadı" · NRF24 "zor çalışıyor" (çipi doğrular ve güç verir) · jam_all 4 bantta · gerçek gönderilen/başarısız sayaçları · WireGuard anahtarı artık seride değil · deauther bağlayıcı.

**✨ Yeni** — Universal IR Remote (markaya göre, kodlar flash'ta, SD yok) · PMKID Capture (klientsiz WPA) · Deauth Detector (savunmacı) · Rolljam (jam+keyfob yakalama) · gerçek MouseJack · deauth hedeflerini kaydet/yükle · yenilenmiş BLE spammer (40 FastPair ID + 18 Apple modeli).

## 💾 Flashe

1. 📥 Kartınız için `.bin` dosyasını [Releases](../../releases) sayfasından indirin
2. 🌐 [ESP Web Flasher](https://espressif.github.io/esptool-js/) açın (Chrome/Edge)
3. **Connect** → ESP32 portunu seçin → **Program** → `.bin` seçin → adres `0x0`
4. 🔄 **RST**'ye basın veya USB'yi yeniden takın

OTA: AP'ye bağlanın (aşağıda), panoyu açın, **OTA Update** → yeni `.bin` seçin.

CLI: `esptool.py --port COM3 --baud 460800 write_flash 0x0 firmware.bin`

## 📡 Panoya bağlanma

İki kimlik bilgisi seti var — hangi özelliği açtığınıza bağlı:

| | AP name | AP password | Dashboard login | IP |
|---|---------|-------------|-----------------|----|
| **🛡️ Arsenal Panosu** | `ArsenalNet` | `arsenal32` | `admin` / `arsenal` | `172.0.0.1` |
| **🌐 Temel Bruce WebUI** | `BruceNet` | `brucenet` | `admin` / `bruce` | `172.0.0.1` |

Hepsi cihazdaki **Ayarlar**'dan yapılandırılabilir. Bir varsayılan çalışmıyorsa biri değiştirmiş — yeniden flashe veya Ayarlar → WiFi AP / WebUI kontrol et. IP **`172.0.0.1`**, `192.168.4.1` değil.

## 🧰 Araçlar (68)

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

✅ = Full ve LITE'da çalışır. Çoğu Full-only araç 16 MB kart gerektirir.

## 🖥️ Kartlar

**Full (16 MB):** T-Embed CC1101 · T-Deck / T-Deck Pro · T-Display S3 (all variants) · T-HMI · T-LoRa Pager · T-Watch S3 · M5Stack Cardputer / CoreS3 / StickS3 · ESP32-S3 DevKitC-1 · Smoochiee

**Full (4 MB):** ESP32-C5 / ESP32-C5 TFT

**LITE (4 MB):** M5Stack Core 4/16 MB · CPlus 1.1 · CYD-2432S028/2USB/W328C/W328R · CYD-3248S035R/C · Elecrow 2.4B/2.8B/3.5B · T-Display TTGO · Marauder Mini/V4-V6/V7/V6.1 · Awok Mini/Touch · WaveSentry R1 · Phantom S024R

**⚠️ Bilinen sorunlar:** M5Stack CPlus2 & Core2 — DRAM overflow, use LITE.

## 🔨 Kaynaktan derle

```bash
git clone https://github.com/quietdom/bruce-arsenal
cd bruce-arsenal
pio run -e <board-name>   # platformio.ini'deki adlar
```

## 👥 Katkıda bulunanlar

- [Bruce Firmware](https://github.com/pr3y/Bruce) — pr3y, temel
- Arsenal modülü + araçlar — quietdom

## ⚖️ Yasal uyarı

Yalnızca yetkili güvenlik testleri ve eğitim amaçlıdır. Bununla aptallık yapmayın.
