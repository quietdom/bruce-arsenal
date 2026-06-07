# Bruce Arsenal — AI Context Transfer Document

## Project Overview
- **Repo:** https://github.com/quietdom/bruce-custom-firmware
- **User:** quietdom
- **Device:** LilyGo T-Embed CC1101 (ESP32-S3, 16MB flash, 8MB PSRAM)
- **Base firmware:** Bruce Firmware (https://github.com/pr3y/Bruce)
- **Platform:** PlatformIO with pioarduino ESP32 platform v55.03.36
- **Build environment:** `lilygo-t-embed-cc1101`
- **Current version:** v1.0.0 (compiled successfully via GitHub Actions)

## What Was Built (v1.0.0)
A custom "Arsenal" module added to Bruce firmware as a new main menu tab (crosshair icon). Contains 26 offensive security tools + background services + mobile web dashboard.

### File locations
- Arsenal source: `src/modules/arsenal/` (28 .cpp/.h files)
- Menu integration: `src/core/menu_items/ArsenalMenu.h` and `ArsenalMenu.cpp`
- Main menu registration: `src/core/main_menu.h` and `main_menu.cpp`
- OPSEC dot injection: `src/core/display.cpp` (added `arsenal_draw_opsec_dot()` call in `drawStatusBar()`)

### v1.0.0 Features (already implemented)
1. Network Scanner (ARP + port scan)
2. DHCP Starvation
3. Karma Attack (wrapper for Bruce's built-in)
4. DNS Spoofer
5. Captive Portal Templates (SD-loaded HTML)
6. Auto-Phish Portal (detect SSID from probes, clone)
7. Credential Forwarding (MITM bridge)
8. WiFi Brute Force (smart wordlist from SSID name)
9. BLE Device Tracker (AirTag/SmartTag detection)
10. BT Name Spammer
11. AirTag Spoofer
12. Bluetooth Audio Jammer
13. Notification Spoofer (Android Fast Pair + Windows Swift Pair + iOS AirPods)
14. Device Fingerprinter (passive OS identification)
15. OPSEC Monitor (deauth detection, threat level)
16. OUI Lookup (offline vendor DB from SD)
17. MAC Rotator
18. Channel Hopper
19. Decoy Traffic Generator
20. Identity Cloner
21. QR Poisoner
22. Jam All (custom multi-band GUI)
23. Attack Scheduler (delay + duration)
24. Session Logger
25. Script Browser (browse 60K+ scripts on SD by category)
26. Remote Web Dashboard (mobile UI + file manager + OTA firmware update)

### Background Services
- OPSEC dot (green/yellow/red on status bar, all screens)
- Always-on evasion (MAC rotate + channel hop + decoy traffic on FreeRTOS task, core 0)
- Low power mode toggle (5x slower intervals)
- Auto-dim during attacks
- Combo presets (built-in + custom from SD `/arsenal/combos/*.txt`)

### Architecture Notes
- Each feature is its own .cpp file in `src/modules/arsenal/`
- `arsenal.h` is the master header (includes `<globals.h>`, `<esp_task_wdt.h>`, `"core/utils.h"`)
- `ARSENAL_SAFE_RUN` macro checks heap before running (but has issues with lambdas containing commas — use inline heap check instead)
- NimBLE is used (not ESP-IDF BLE) — use `ble_hs_id_set_rnd()` for random address, declared as `extern "C"` at file top
- `addOptionToMainMenu()` comes from `core/utils.h`
- The BLE tracker uses `BLEAdvertisedDeviceCallbacks` from NimBLE-Arduino v2.5
- Web dashboard uses AsyncWebServer on port 80 with AP mode (`ArsenalNet` / `arsenal32`)
- Script browser reads from `/arsenal/{category}/` folders on SD
- Background task runs on `xTaskCreatePinnedToCore(..., core 0)`

### Build Notes
- platformio.ini `build_src_flags`: `-w -fpermissive -Wno-narrowing`
- platformio.ini `build_flags` includes: `-w -fpermissive -Wno-narrowing -Wno-error -Wl,--allow-multiple-definition`
- The `-Wl,--allow-multiple-definition` is REQUIRED because NimBLE's FreeRTOS wrappers conflict with the framework's built-in ones
- The `-flto` flag is present and required (project has `flto_prep.py` script)
- `mykeyboard.cpp` line 515 has a narrowing fix: `(int)(9.5 * LW * FM)`
- Platform.py was patched to skip `setup_penv_minimal` (replaced with `penv_python = sys.executable; esptool_path = "esptool"`)
- Fake `fatfs` Python module created at `penv/Lib/site-packages/fatfs/` with import hook for all submodules
- GitHub Actions workflow `manual_build_sel_env.yml` successfully compiles the project

### SD Card Structure
```
/arsenal/
├── badusb/       .txt DuckyScript
├── subghz/       .sub signals
├── ir/           .ir remotes
├── portals/      .html evil portals
├── nfc/          .nfc dumps
├── rfid/         .rfid captures
├── ibutton/      .ibutton keys
├── scripts/      .js automation
├── combos/       .txt combo presets
├── logs/         session logs (auto)
├── config.txt    AP name/password (v1.0.1)
└── creds.txt     captured credentials (auto)
```

---

## v1.0.1 TODO — 46 New Features to Build

### Config / QoL
1. Configurable AP name + password (from `/arsenal/config.txt`)
2. Configurable dashboard login credentials
3. PIN lock on Arsenal menu (4-digit)

### WiFi Attacks
4. WPA Handshake grabber (deauth + capture .hccapx)
5. Beacon flood (100+ fake SSIDs)
6. Selective client deauth (target one device)
7. Auth flood (overload AP client table)
8. AP clone flood (50 copies of target AP)
9. ARP poisoner (classic MITM on shared network)
10. SSL strip lite (downgrade HTTPS in portal redirects)
11. UPnP port opener (open firewall ports without auth)
12. Default credential scanner (try admin/admin on discovered devices)
13. DNS tunnel (exfiltrate data via DNS queries)
14. WPS PIN attack (try common PINs)
15. Rogue AP detector (alert if network name cloned)

### RF / Sub-GHz
16. NRF24 MouseJack (inject keystrokes into wireless keyboards)
17. Wireless doorbell replay (433MHz capture + replay)
18. Garage brute force (all 4096 12-bit codes)
19. Car keyfob logger (log frequency + timestamp, no replay)
20. Frequency scanner (sweep 300-928MHz, show signal strength)
21. Flipper .sub file import + replay

### BLE
22. Bluetooth audio rickroll (connect to unprotected speakers)
23. Bluetooth device profiler (enumerate GATT services)

### Phishing Portals
24. Fake Windows Update portal
25. WiFi speed test phish
26. OAuth login phish (Google/Microsoft)
27. "Device found" portal

### Intelligence / Recon
28. WiFi client probe log (silent passive collection)
29. SSID history logger (map what networks everyone carries)
30. Service banner grabber (SSH/HTTP/FTP version strings)
31. Smart home scanner (find Tuya/Tasmota/ESPHome devices)
32. WiFi channel utilization chart (visual per-channel load)
33. People counter (unique MACs over time)
34. Device nickname system (assign names to MACs)

### Detection
35. Flipper Zero detector (detect Flipper BLE signature + SubGHz patterns)
36. Hacking device detector (Flipper, HackRF, WiFi Pineapple, other ESP32 pentest tools)
37. RF silence enforcer (detect if YOUR devices are transmitting when they shouldn't)

### Comms
38. ESP-NOW encrypted chat (P2P between Bruce devices)
39. ESP-NOW C2 (control multiple devices from one master)
40. Dead drop mesh (encrypted messages on SD for other devices)
41. IR data transfer (send files via IR, no wireless trace)
42. Multi-device sync over ESP-NOW

### Evasion
43. Time-based attack randomizer (vary timing to avoid IDS patterns)

### Utility
44. Password generator (random secure passwords on screen)
45. NFC business card emulator (tap to open URL)
46. Attack stats dashboard (lifetime totals: creds captured, devices seen, attacks run)

---

## User Preferences
- Likes: minimal code comments, no AI-looking decorative blocks
- Dislikes: "Coming Soon" stubs, verbose/chatty code comments
- Style: nonchalant, clean, tuff
- Hardware arrives: June 15, 2026
- GitHub: quietdom
- Socials: guns.lol/quietdom_
- Builds via: GitHub Actions (manual_build_sel_env workflow)
- Target board: lilygo-t-embed-cc1101
- README has 14 language translations in /docs/
