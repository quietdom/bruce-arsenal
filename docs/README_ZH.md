🌐 [English](../README.md) | [Português](README_PT.md) | [Italiano](README_IT.md) | [Français](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Türkçe](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Español](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>ESP32 设备自定义攻击性安全固件</b><br>
  <i>26 款攻击工具 • 移动仪表板 • 后台规避 • 60K+ 脚本</i>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32--S3-blue?style=flat-square" alt="ESP32-S3">
  <img src="https://img.shields.io/badge/Target-T--Embed%20CC1101-green?style=flat-square" alt="T-Embed">
  <img src="https://img.shields.io/badge/Tools-26+-red?style=flat-square" alt="Tools">
  <img src="https://img.shields.io/badge/Scripts-60K+-purple?style=flat-square" alt="Scripts">
  <img src="https://img.shields.io/badge/Status-Active-brightgreen?style=flat-square" alt="Status">
</p>

---

## 这是什么

这是 [Bruce Firmware](https://github.com/pr3y/Bruce) 的深度修改版本，集成了自定义 **Arsenal** 模块 —— 一个完整的攻击性安全工具包，可从带有准星图标的单个菜单标签访问。

所有 Bruce 原始功能保持不变。Arsenal 增加了 26 个新工具、移动 Web 仪表板、后台规避服务以及可从 SD 卡加载 60K+ 载荷的脚本浏览器。

---

## 安装

1. 从 [Releases](../../releases) 下载 `.bin` 文件
2. 使用以下方法之一刷入设备：
   - **USB：** [ESP Web Flasher](https://espressif.github.io/esptool-js/)（Chrome/Edge）
   - **OTA：** 首次刷入后，使用 Arsenal → Dashboard → 从手机上传新 .bin
3. 完成

---

## Arsenal 菜单

```
🎯 Arsenal
├── WiFi Arsenal ──────── 8 款攻击性 WiFi 工具
├── BLE Arsenal ───────── 5 款蓝牙攻击工具
├── 情报 ──────────────── 3 款侦察/感知工具
├── 规避 ──────────────── 5 款隐蔽/反检测工具
├── 组合 ──────────────── 预设 + 自定义攻击链
├── 调度器 ────────────── 延时攻击执行
├── 脚本 ──────────────── 浏览 SD 卡中 60K+ 载荷
├── 会话日志 ──────────── SD 卡活动记录
├── Jam All ───────────── 多频段同时干扰（自定义 GUI）
└── Dashboard ─────────── 移动 Web UI + 文件管理器 + OTA
```

---

## 工具

### WiFi Arsenal

| 工具 | 功能描述 |
|------|----------|
| **Network Scanner** | ARP 扫描 + TCP 端口扫描。显示设备、开放端口、厂商名称 |
| **DHCP Starvation** | 使用伪造 MAC 地址发送大量 DHCP 请求以耗尽路由器 IP 池 |
| **Karma Attack** | 响应所有 WiFi probe 请求 —— 附近设备自动连接到你 |
| **DNS Spoofer** | 以 AP 模式运行时拦截 DNS。所有域名解析到你的门户 |
| **Portal Templates** | 从 SD 卡加载自定义钓鱼 HTML。将凭据捕获到文件 |
| **Auto-Phish Portal** | 检测受害者正在搜索的网络，克隆 SSID，提供门户 |
| **Cred Forward** | 捕获门户凭据 → 自动连接到真实 AP → 桥接流量。完全透明 MITM |
| **WiFi Brute Force** | 根据 SSID 名称生成智能字典（年份组合、leet speak、常见模式）。实时尝试 |

### BLE Arsenal

| 工具 | 功能描述 |
|------|----------|
| **BLE Tracker** | 持续 BLE 扫描，带 RSSI 距离估算。检测跟踪你的 AirTag、SmartTag、Tile |
| **Name Spammer** | 用数百个随机设备名称淹没 BLE 广播信道 |
| **AirTag Spoofer** | 广播虚假 Apple FindMy AirTag 信标。幻影标签出现在附近的 iPhone 上 |
| **Audio Jammer** | 通过广播洪水干扰 BLE 音频连接（AirPods、扬声器） |
| **Notification Spoofer** | 向 Android（Fast Pair）、Windows（Swift Pair）和 iOS（AirPods 近距离）推送虚假设备发现弹窗 |

### 情报

| 工具 | 功能描述 |
|------|----------|
| **Device Fingerprinter** | 被动 WiFi 指纹识别。通过 probe 行为、OUI、时序识别设备 OS/型号 |
| **OPSEC Monitor** | 检测你是否被扫描、去认证或追踪。威胁等级：绿色/黄色/红色 |
| **OUI Lookup** | 从 SD 卡离线查询 MAC 厂商数据库 |

### 规避

| 工具 | 功能描述 |
|------|----------|
| **MAC Rotator** | 在可配置定时器上随机化 WiFi MAC 地址。速度可调 |
| **Channel Hopper** | 操作期间快速切换 WiFi 信道。间隔可配置 |
| **Decoy Traffic** | 在轮换信道上发射随机 beacon/probe 帧。用噪声掩盖真实活动 |
| **Identity Cloner** | 捕获附近设备身份（MAC + probe 模式）。克隆一个以变得隐形 |
| **QR Poisoner** | 生成指向你的门户的 QR 码。在屏幕上显示、打印并放置 |

### Jam All（自定义 GUI）

全屏仪表板，带有逐频段切换、实时活动条、已用时间计时器和一键停止按钮。

| 频段 | 方法 |
|------|------|
| WiFi 2.4GHz | 循环所有 14 个信道的 deauth 洪水 |
| BLE | 广播信道洪水（37、38、39） |
| Sub-GHz | CC1101 连续发射（315/433/868/915 MHz） |
| NRF24 | 轮换信道上的 2.4GHz 载波 |

### 实用工具

| 工具 | 功能描述 |
|------|----------|
| **Attack Scheduler** | 选择工具 → 设置延迟（0 到 1 小时）→ 设置持续时间。自动启动和停止 |
| **Session Log** | 记录每个操作及时间戳。从菜单查看/导出/清除 |
| **Script Browser** | 浏览 SD 卡分类文件夹。根据文件类型自动启动（.txt/.sub/.ir/.html/.nfc/.js） |
| **Combo Presets** | 一键攻击链。内置：Stealth Mode、Full Attack、Passive Recon。在 SD 卡上创建自定义 .txt |
| **Remote Dashboard** | 手机连接到 ArsenalNet AP → 完整移动 Web UI，带文件管理器 + OTA 更新 |

---

## 后台服务

这些服务在所有菜单中持续运行 —— 不仅仅在 Arsenal 内部。

| 服务 | 描述 |
|------|------|
| **OPSEC Dot** | 状态栏中的彩色圆点（绿色/黄色/红色）。始终可见，每个界面 |
| **Always-On Evasion** | MAC 轮换 + channel hopping + decoy traffic。在 CPU 核心 0 上静默运行 |
| **Low Power Mode** | 将后台任务减慢 5 倍以节省电量。从 Combos 菜单切换 |
| **Auto-Dim** | 活动攻击期间屏幕亮度降低。停止后恢复 |

---

## Remote Dashboard

启动 Arsenal → Dashboard。将手机连接到 `ArsenalNet`（密码：`arsenal32`）。在浏览器中打开 `192.168.4.1`。

**功能：**
- 从手机启动/停止所有 26 个工具
- 浏览、上传、删除 SD 卡上的文件（`/arsenal/` 文件夹）
- 上传 `.bin` 固件进行 OTA 更新
- 拖放上传脚本到分类文件夹
- 实时系统状态（堆内存、SD 空间、运行时间）

---

## SD 卡布局

```
/arsenal/
├── badusb/       DuckyScript 载荷 (.txt)
├── subghz/       Sub-GHz 信号捕获 (.sub)
├── ir/           IR 遥控文件 (.ir)
├── portals/      Evil portal HTML 模板
├── nfc/          NFC 卡转储 (.nfc)
├── rfid/         125kHz RFID 捕获 (.rfid)
├── ibutton/      iButton 密钥文件 (.ibutton)
├── scripts/      JavaScript 自动化 (.js)
├── combos/       自定义组合预设 (.txt — 每行一个功能名称)
├── logs/         会话日志（自动创建）
└── creds.txt     捕获的凭据（自动创建）
```

兼容 [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts)（60K+ 脚本）。

---

## 自定义组合

在 `/arsenal/combos/` 中创建 `.txt` 文件，每行一个功能：

```
mac_rotator
channel_hopper
decoy_traffic
ble_tracker
```

自动出现在 Arsenal → Combos 中。

---

## 硬件

主要目标：**LilyGo T-Embed CC1101**
- ESP32-S3（16MB flash，8MB PSRAM）
- ST7789 320x170 TFT 显示屏
- 旋转编码器 + 按钮
- CC1101 Sub-GHz 射频
- NRF24 2.4GHz 射频
- PN532 NFC
- SD 卡插槽
- USB-C（BadUSB HID）
- 扬声器 + 麦克风
- RGB LED
- BQ27220 电量计

---

## 致谢

- 基于 [Bruce Firmware](https://github.com/pr3y/Bruce)，作者 pr3y
- Arsenal 模块，作者 quietdom
- 脚本集兼容 [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts)

---

## 免责声明

本固件仅用于授权的安全测试和教育目的。请勿对您不拥有或未获得明确测试许可的网络或设备使用。作者对滥用不承担任何责任。
