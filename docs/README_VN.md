🌐 [English](../README.md) | [Português](README_PT.md) | [Italiano](README_IT.md) | [Français](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Türkçe](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Español](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>Firmware tấn công bảo mật tùy chỉnh cho thiết bị ESP32</b><br>
  <i>26 công cụ tấn công • Bảng điều khiển di động • Né tránh nền • 60K+ script</i>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32--S3-blue?style=flat-square" alt="ESP32-S3">
  <img src="https://img.shields.io/badge/Target-T--Embed%20CC1101-green?style=flat-square" alt="T-Embed">
  <img src="https://img.shields.io/badge/Tools-26+-red?style=flat-square" alt="Tools">
  <img src="https://img.shields.io/badge/Scripts-60K+-purple?style=flat-square" alt="Scripts">
  <img src="https://img.shields.io/badge/Status-Active-brightgreen?style=flat-square" alt="Status">
</p>

---

## Đây Là Gì

Phiên bản được chỉnh sửa sâu của [Bruce Firmware](https://github.com/pr3y/Bruce) với module **Arsenal** tùy chỉnh được tích hợp — bộ công cụ bảo mật tấn công đầy đủ có thể truy cập từ một tab menu duy nhất với biểu tượng ống ngắm.

Tất cả tính năng gốc của Bruce vẫn được giữ nguyên. Arsenal bổ sung 26 công cụ mới, bảng điều khiển web di động, dịch vụ né tránh nền và trình duyệt script có thể tải 60K+ payload từ thẻ SD.

---

## Cài Đặt

1. Tải file `.bin` từ [Releases](../../releases)
2. Flash vào thiết bị bằng một trong các cách:
   - **USB:** [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
   - **OTA:** Sau lần flash đầu tiên, sử dụng Arsenal → Dashboard → tải .bin mới từ điện thoại
3. Xong

---

## Menu Arsenal

```
🎯 Arsenal
├── WiFi Arsenal ──────── 8 công cụ tấn công WiFi
├── BLE Arsenal ───────── 5 công cụ tấn công Bluetooth
├── Tình Báo ──────────── 3 công cụ trinh sát/nhận biết
├── Né Tránh ──────────── 5 công cụ tàng hình/chống phát hiện
├── Combo ─────────────── Chuỗi tấn công đặt sẵn + tùy chỉnh
├── Lập Lịch ──────────── Thực thi tấn công có trì hoãn
├── Script ────────────── Duyệt 60K+ payload từ SD
├── Nhật Ký Phiên ─────── Ghi lại hoạt động vào SD
├── Jam All ───────────── Gây nhiễu đồng thời đa băng tần (GUI tùy chỉnh)
└── Dashboard ─────────── UI web di động + quản lý file + OTA
```

---

## Công Cụ

### WiFi Arsenal

| Công Cụ | Chức Năng |
|---------|-----------|
| **Network Scanner** | Quét ARP + quét cổng TCP. Hiển thị thiết bị, cổng mở, tên nhà sản xuất |
| **DHCP Starvation** | Gửi tràn yêu cầu DHCP với MAC giả để cạn kiệt pool IP của router |
| **Karma Attack** | Phản hồi tất cả yêu cầu WiFi probe — thiết bị gần đó tự động kết nối với bạn |
| **DNS Spoofer** | Chặn DNS khi chạy dưới dạng AP. Tất cả domain dẫn đến portal của bạn |
| **Portal Templates** | Tải HTML phishing tùy chỉnh từ thẻ SD. Bắt thông tin đăng nhập vào file |
| **Auto-Phish Portal** | Phát hiện mạng nào nạn nhân đang tìm, clone SSID, phục vụ portal |
| **Cred Forward** | Bắt thông tin portal → tự động kết nối AP thật → bridge traffic. MITM trong suốt hoàn toàn |
| **WiFi Brute Force** | Tạo wordlist thông minh từ tên SSID (kết hợp năm, leet speak, pattern phổ biến). Thử trực tiếp |

### BLE Arsenal

| Công Cụ | Chức Năng |
|---------|-----------|
| **BLE Tracker** | Quét BLE liên tục với ước lượng khoảng cách RSSI. Phát hiện AirTag, SmartTag, Tile đang theo dõi bạn |
| **Name Spammer** | Gửi tràn kênh advertising BLE với hàng trăm tên thiết bị ngẫu nhiên |
| **AirTag Spoofer** | Phát beacon Apple FindMy AirTag giả. Tag ảo xuất hiện trên iPhone gần đó |
| **Audio Jammer** | Phá vỡ kết nối âm thanh BLE (AirPods, loa) bằng tràn advertisement |
| **Notification Spoofer** | Đẩy popup giả phát hiện thiết bị lên Android (Fast Pair), Windows (Swift Pair) và iOS (AirPods proximity) |

### Tình Báo

| Công Cụ | Chức Năng |
|---------|-----------|
| **Device Fingerprinter** | Fingerprinting WiFi thụ động. Nhận dạng OS/model thiết bị từ hành vi probe, OUI, timing |
| **OPSEC Monitor** | Phát hiện nếu bạn đang bị quét, deauth hoặc theo dõi. Mức đe dọa: xanh/vàng/đỏ |
| **OUI Lookup** | Tra cứu offline database nhà sản xuất MAC từ thẻ SD |

### Né Tránh

| Công Cụ | Chức Năng |
|---------|-----------|
| **MAC Rotator** | Ngẫu nhiên hóa địa chỉ MAC WiFi theo bộ đếm thời gian cấu hình được. Tốc độ điều chỉnh được |
| **Channel Hopper** | Chuyển nhanh kênh WiFi trong quá trình hoạt động. Khoảng cách cấu hình được |
| **Decoy Traffic** | Phát frame beacon/probe ngẫu nhiên trên các kênh xoay vòng. Che giấu hoạt động thực trong nhiễu |
| **Identity Cloner** | Bắt danh tính thiết bị gần đó (MAC + pattern probe). Clone một để trở nên vô hình |
| **QR Poisoner** | Tạo mã QR trỏ đến portal của bạn. Hiển thị trên màn hình, in ra và đặt |

### Jam All (GUI Tùy Chỉnh)

Bảng điều khiển toàn màn hình với toggle theo từng băng, thanh hoạt động trực tiếp, bộ đếm thời gian và nút dừng khẩn cấp.

| Băng Tần | Phương Pháp |
|----------|-------------|
| WiFi 2.4GHz | Tràn deauth xoay vòng tất cả 14 kênh |
| BLE | Tràn kênh advertisement (37, 38, 39) |
| Sub-GHz | CC1101 TX liên tục (315/433/868/915 MHz) |
| NRF24 | Sóng mang 2.4GHz trên các kênh xoay vòng |

### Tiện Ích

| Công Cụ | Chức Năng |
|---------|-----------|
| **Attack Scheduler** | Chọn công cụ → đặt độ trễ (0 đến 1 giờ) → đặt thời lượng. Tự khởi động và dừng |
| **Session Log** | Ghi lại mọi hành động với timestamp. Xem/xuất/xóa từ menu |
| **Script Browser** | Duyệt thư mục SD đã phân loại. Tự khởi chạy theo loại file (.txt/.sub/.ir/.html/.nfc/.js) |
| **Combo Presets** | Chuỗi tấn công một chạm. Tích hợp sẵn: Stealth Mode, Full Attack, Passive Recon. Tạo tùy chỉnh .txt trên SD |
| **Remote Dashboard** | Điện thoại kết nối AP ArsenalNet → UI web di động đầy đủ với quản lý file + cập nhật OTA |

---

## Dịch Vụ Nền

Các dịch vụ này chạy liên tục trong tất cả menu — không chỉ bên trong Arsenal.

| Dịch Vụ | Mô Tả |
|----------|--------|
| **OPSEC Dot** | Chấm tròn màu trên thanh trạng thái (xanh/vàng/đỏ). Luôn hiển thị, mọi màn hình |
| **Always-On Evasion** | Xoay MAC + channel hopping + decoy traffic. Chạy im lặng trên lõi CPU 0 |
| **Low Power Mode** | Giảm tốc tác vụ nền 5x để tiết kiệm pin. Bật/tắt từ menu Combos |
| **Auto-Dim** | Độ sáng màn hình giảm khi tấn công đang hoạt động. Khôi phục khi dừng |

---

## Remote Dashboard

Khởi động Arsenal → Dashboard. Kết nối điện thoại vào `ArsenalNet` (mật khẩu: `arsenal32`). Mở `192.168.4.1` trong trình duyệt.

**Tính năng:**
- Khởi động/dừng tất cả 26 công cụ từ điện thoại
- Duyệt, tải lên, xóa file trên thẻ SD (thư mục `/arsenal/`)
- Tải lên firmware `.bin` để cập nhật OTA
- Kéo thả upload script vào thư mục phân loại
- Trạng thái hệ thống trực tiếp (heap, dung lượng SD, thời gian hoạt động)

---

## Bố Cục Thẻ SD

```
/arsenal/
├── badusb/       Payload DuckyScript (.txt)
├── subghz/       Bắt tín hiệu Sub-GHz (.sub)
├── ir/           File điều khiển IR (.ir)
├── portals/      Template HTML evil portal
├── nfc/          Dump thẻ NFC (.nfc)
├── rfid/         Bắt RFID 125kHz (.rfid)
├── ibutton/      File khóa iButton (.ibutton)
├── scripts/      Tự động hóa JavaScript (.js)
├── combos/       Preset combo tùy chỉnh (.txt — mỗi dòng một tên tính năng)
├── logs/         Nhật ký phiên (tự động tạo)
└── creds.txt     Thông tin đăng nhập bắt được (tự động tạo)
```

Tương thích với [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts) (60K+ script).

---

## Combo Tùy Chỉnh

Tạo file `.txt` trong `/arsenal/combos/` với mỗi dòng một tính năng:

```
mac_rotator
channel_hopper
decoy_traffic
ble_tracker
```

Tự động xuất hiện trong Arsenal → Combos.

---

## Phần Cứng

Mục tiêu chính: **LilyGo T-Embed CC1101**
- ESP32-S3 (16MB flash, 8MB PSRAM)
- Màn hình TFT ST7789 320x170
- Encoder xoay + nút bấm
- Radio Sub-GHz CC1101
- Radio NRF24 2.4GHz
- NFC PN532
- Khe cắm thẻ SD
- USB-C (BadUSB HID)
- Loa + Microphone
- LED RGB
- Đo pin BQ27220

---

## Ghi Nhận

- Dựa trên [Bruce Firmware](https://github.com/pr3y/Bruce) bởi pr3y
- Module Arsenal bởi quietdom
- Bộ sưu tập script tương thích với [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts)

---

## Tuyên Bố Miễn Trừ

Firmware này chỉ dành cho kiểm tra bảo mật được ủy quyền và mục đích giáo dục. Không sử dụng chống lại mạng hoặc thiết bị mà bạn không sở hữu hoặc không có sự cho phép rõ ràng để kiểm tra. Các tác giả không chịu trách nhiệm về việc sử dụng sai mục đích.
