?? [English](../README.md) | [Portugues](README_PT.md) | [Italiano](README_IT.md) | [Francais](README_FR.md) | [Russian](README_RU.md) | [Chinese](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Turkce](README_TR.md) | [Deutsch](README_DE.md) | [Vietnamese](README_VN.md) | [Espanol](README_ES.md) | [Indonesia](README_ID.md) | [Arabic](README_AR.md)

<h1 align="center">?? Bruce Arsenal</h1>

<p align="center">
  <b>ESP32设备自定义攻击性安全固件</b><br>
  <i>72个工具 - 一个固件 - 多个设备</i>
</p>

---

## 这是什么

Bruce Firmware的修改版本，带有自定义Arsenal模块 - 72个攻击性安全工具，可从单个菜单访问。

---

## 安装

1. 从Releases下载.bin文件
2. 通过ESP Web Flasher (Chrome/Edge)或通过Arsenal Dashboard OTA刷写

---

## Arsenal工具 (72)

### WiFi Arsenal (11)
Network Scanner, DHCP Starvation, Karma Attack, DNS Spoofer, Auto-Phish Portal, Cred Forward, Auth Flood, AP Clone Flood, SSL Strip Lite, DNS Tunnel, WPS PIN Attack

### BLE Arsenal (6)
BLE Tracker, BT Name Spammer, AirTag Spoofer, Audio Jammer, Notification Spoofer, BT Rickroll

### 情报 (8)
Device Fingerprinter, OPSEC Monitor, OUI Lookup, Probe Log, Banner Grabber, SmartHome Scan, Channel Chart, People Counter

### 规避 (5)
MAC Rotator, Channel Hopper, Decoy Traffic, Identity Cloner, Time Randomizer

### 钓鱼 (4)
Win Update Phish, WiFi Speed Phish, OAuth Phish, Device Found Phish

### Sub-GHz / RF (6)
NRF24 MouseJack, Doorbell Replay, Garage Brute Force, Keyfob Logger, Frequency Scanner, Flipper Import

### 检测 (3)
Flipper Detector, Hacker Detector, RF Silence Enforcer

### 通信 (5)
ESP-NOW Chat, ESP-NOW C2, Dead Drop Mesh, IR Data Transfer, Multi-Device Sync

### 实用工具 (2)
NFC Biz Card, Attack Stats

---

## 开发板兼容性

### ESP32-S3 (完整版 - 72 个工具)
LilyGo T-Embed CC1101, T-Deck Pro, T-Deck, T-Display S3, T-HMI, T-LoRa Pager, T-Watch S3, M5Stack Cardputer, CoreS3, StickS3

### ESP32 (LITE - 31 个工具)
M5Stack Core 4MB/16MB, CPlus 1.1, CYD (all variants), Elecrow, Marauder, Awok, WaveSentry, Phantom

### 已知限制
M5Stack CPlus2, M5Stack Core2 - DRAM溢出（核心固件全局变量）

---

## 后台服务

- **OPSEC Dot** - 状态栏上的彩色圆点
- **Always-On Evasion** - MAC轮换 + 频道跳跃 + 诱饵流量
- **Auto-Dim** - 攻击期间屏幕亮度降低

---

## 远程仪表板

Arsenal > Dashboard. 将手机连接到ArsenalNet（密码：arsenal32）。打开192.168.4.1。

---

## 致谢

- [Bruce Firmware](https://github.com/pr3y/Bruce) by pr3y
- Arsenal module by quietdom

---

## 仅用于授权安全测试和教育目的。