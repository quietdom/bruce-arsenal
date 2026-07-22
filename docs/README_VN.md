🌐 [English](../README.md) | [Portugues](README_PT.md) | [Italiano](README_IT.md) | [Francais](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Turkce](README_TR.md) | [Deutsch](README_DE.md) | **Tiếng Việt** | [Espanol](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/artworks/bruce-working.jpg" alt="Bruce Arsenal" width="500">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>Firmware bảo mật tấn công tùy chỉnh cho ESP32</b><br>
  <i>Một firmware, nhiều board — 68 công cụ</i>
</p>

---

Một phiên bản sửa đổi của [Bruce Firmware](https://github.com/pr3y/Bruce) với module **Arsenal** tùy chỉnh. Tất cả tính năng Bruce gốc được giữ nguyên. Biên dịch và kiểm tra trên 30 board trước mỗi bản release.

## Có gì mới trong v1.2.0

**Đã sửa** — Treo RF Listen trên T-Embed (giải phóng bus SPI dùng chung khi thoát) ·"Không tìm thấy CC1101"·NRF24"hầu như không hoạt động"(kiểm tra chip và cấp nguồn)·jam_all 4 băng tần·bộ đếm gửi/thất bại thật·khóa WireGuard không còn in ra serial·linker deauther.

**Mới** — Universal IR Remote (theo hãng, mã trong flash, không cần SD)·PMKID Capture (WPA không cần client)·Deauth Detector (phòng thủ)·Rolljam (jam+bắt keyfob)·MouseJack thật·lưu/tải mục tiêu deauth·BLE spammer mới (40 FastPair ID + 18 mẫu Apple).

## Nạp firmware

1. Tải `.bin` cho board của bạn từ [Releases](../../releases)
2. Mở [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
3. **Connect** → chọn cổng ESP32 → **Program** → chọn `.bin` → địa chỉ `0x0`
4. Nhấn **RST** hoặc cắm lại USB

OTA: kết nối AP (dưới đây), mở bảng điều khiển, **OTA Update** → chọn `.bin` mới.

CLI: `esptool.py --port COM3 --baud 460800 write_flash 0x0 firmware.bin`

## Kết nối bảng điều khiển

Có hai bộ thông tin đăng nhập — tùy thuộc vào tính năng bạn mở:

| | AP name | AP password | Dashboard login | IP |
|---|---------|-------------|-----------------|----|
| **Bảng Arsenal** | `ArsenalNet` | `arsenal32` | `admin` / `arsenal` | `172.0.0.1` |
| **WebUI Bruce cơ sở** | `BruceNet` | `brucenet` | `admin` / `bruce` | `172.0.0.1` |

Tất cả cấu hình được trong **Cài đặt** trên thiết bị. Nếu mặc định không hoạt động, ai đó đã đổi — nạp lại hoặc kiểm tra Cài đặt → WiFi AP / WebUI. IP là **`172.0.0.1`**, không phải `192.168.4.1`.

## Công cụ (68)

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

✅ = hoạt động trên cả Full và LITE. Hầu hết Full-only cần board 16 MB.

## Board

**Full (16 MB):** T-Embed CC1101 · T-Deck / T-Deck Pro · T-Display S3 (all variants) · T-HMI · T-LoRa Pager · T-Watch S3 · M5Stack Cardputer / CoreS3 / StickS3 · ESP32-S3 DevKitC-1 · Smoochiee

**Full (4 MB):** ESP32-C5 / ESP32-C5 TFT

**LITE (4 MB):** M5Stack Core 4/16 MB · CPlus 1.1 · CYD-2432S028/2USB/W328C/W328R · CYD-3248S035R/C · Elecrow 2.4B/2.8B/3.5B · T-Display TTGO · Marauder Mini/V4-V6/V7/V6.1 · Awok Mini/Touch · WaveSentry R1 · Phantom S024R

**Vấn đề đã biết:** M5Stack CPlus2 & Core2 — DRAM overflow, use LITE.

## Biên dịch từ mã nguồn

```bash
git clone https://github.com/quietdom/bruce-arsenal
cd bruce-arsenal
pio run -e <board-name>   # tên trong platformio.ini
```

## Cảm ơn

- [Bruce Firmware](https://github.com/pr3y/Bruce) bởi pr3y — nền tảng
- Module Arsenal + công cụ bởi quietdom

## Miễn trừ trách nhiệm

Chỉ dùng cho kiểm tra bảo mật được ủy quyền và mục đích giáo dục. Đừng làm ngu với cái này.
