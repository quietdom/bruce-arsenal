🌐 [English](../README.md) | [Portugues](README_PT.md) | [Italiano](README_IT.md) | [Francais](README_FR.md) | [Русский](README_RU.md) | **中文** | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Turkce](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Espanol](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/artworks/bruce-working.jpg" alt="Bruce Arsenal" width="500">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>ESP32设备的自定义攻击性安全固件</b><br>
  <i>一个固件，多种设备 — 68个工具</i>
</p>

---

经过修改的 [Bruce Firmware](https://github.com/pr3y/Bruce)，带有自定义 **Arsenal** 模块。所有原始 Bruce 功能保持不变。每次发布前在30块板子上编译和测试。

## v1.2.0 新功能

**已修复** — T-Embed RF Listen 冻结（退出时释放共享SPI总线）·"CC1101 未找到"·NRF24"勉强能用"（验证芯片并供电）·jam_all 四个频段·真实的发送/失败计数器·WireGuard私钥不再输出到串口·deauther链接器。

**新增** — Universal IR Remote（按品牌，代码在flash，无需SD）·PMKID Capture（无客户端WPA）·Deauth Detector（防御性）·Rolljam（jam+捕获遥控器）·真正的MouseJack·保存/加载deauth目标·更新的BLE spammer（40个FastPair ID + 18个Apple型号）。

## 烧录

1. 从 [Releases](../../releases) 下载你板子的 `.bin`
2. 打开 [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
3. **Connect** → 选择 ESP32 端口 → **Program** → 选择 `.bin` → 地址 `0x0`
4. 按 **RST** 或重新插拔 USB

OTA：连接AP（见下），打开面板，**OTA Update** → 选择新的 `.bin`。

CLI: `esptool.py --port COM3 --baud 460800 write_flash 0x0 firmware.bin`

## 连接到面板

有两套凭据 — 取决于你打开了哪个功能：

| | AP name | AP password | Dashboard login | IP |
|---|---------|-------------|-----------------|----|
| **Arsenal面板** | `ArsenalNet` | `arsenal32` | `admin` / `arsenal` | `172.0.0.1` |
| **基础Bruce WebUI** | `BruceNet` | `brucenet` | `admin` / `bruce` | `172.0.0.1` |

全部可在设备的**设置**中配置。如果默认值不工作，有人改过 — 重新烧录或检查设置 → WiFi AP / WebUI。IP是 **`172.0.0.1`**，不是 `192.168.4.1`。

## 工具 (68)

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

✅ = Full和LITE都能用。大多数Full-only工具需要16 MB板子。

## 开发板

**Full (16 MB):** T-Embed CC1101 · T-Deck / T-Deck Pro · T-Display S3 (all variants) · T-HMI · T-LoRa Pager · T-Watch S3 · M5Stack Cardputer / CoreS3 / StickS3 · ESP32-S3 DevKitC-1 · Smoochiee

**Full (4 MB):** ESP32-C5 / ESP32-C5 TFT

**LITE (4 MB):** M5Stack Core 4/16 MB · CPlus 1.1 · CYD-2432S028/2USB/W328C/W328R · CYD-3248S035R/C · Elecrow 2.4B/2.8B/3.5B · T-Display TTGO · Marauder Mini/V4-V6/V7/V6.1 · Awok Mini/Touch · WaveSentry R1 · Phantom S024R

**已知问题:** M5Stack CPlus2 & Core2 — DRAM overflow, use LITE.

## 从源码编译

```bash
git clone https://github.com/quietdom/bruce-arsenal
cd bruce-arsenal
pio run -e <board-name>   # 名字在 platformio.ini
```

## 鸣谢

- [Bruce Firmware](https://github.com/pr3y/Bruce) 作者 pr3y — 基础
- Arsenal模块 + 工具 — quietdom

## 免责声明

仅用于授权的安全测试和教育目的。别拿这个犯蠢。
