🌐 [English](../README.md) | **Portugues** | [Italiano](README_IT.md) | [Francais](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Turkce](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Espanol](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/artworks/bruce-working.jpg" alt="Bruce Arsenal" width="500">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>Firmware de segurança ofensiva personalizado para ESP32</b><br>
  <i>Um firmware, várias placas — 68 ferramentas</i>
</p>

---

Uma modificação do [Bruce Firmware](https://github.com/pr3y/Bruce) com um módulo **Arsenal** personalizado. Todas as funcionalidades originais do Bruce permanecem intactas. Compilado e testado em 30 placas antes de cada release.

## Novidades na v1.2.0

**Corrigido** — Freeze do RF Listen no T-Embed (barramento SPI compartilhado liberado na saída) · "CC1101 não encontrado" · NRF24 "mal funciona" (verifica o chip e liga direito) · jam_all nas 4 bandas · contadores reais de enviado/falha · chave WireGuard não vaza no serial · linker do deauther.

**Novo** — Universal IR Remote (por marca, códigos em flash, sem SD) · PMKID Capture (WPA sem cliente) · Deauth Detector (defensivo) · Rolljam (jam+captura de keyfob) · MouseJack real · salvar/carregar alvos de deauth · BLE spammer renovado (40 FastPair IDs + 18 modelos Apple).

## Gravar

1. Baixe o `.bin` da sua placa em [Releases](../../releases)
2. Abra o [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
3. **Connect** → escolha a porta ESP32 → **Program** → escolha o `.bin` → endereço `0x0`
4. Pressione **RST** ou reconecte o USB

OTA: conecte ao AP (abaixo), abra o dashboard, **OTA Update** → escolha o novo `.bin`.

CLI: `esptool.py --port COM3 --baud 460800 write_flash 0x0 firmware.bin`

## Conectando ao dashboard

Existem dois conjuntos de credenciais — dependem de qual recurso você abriu:

| | AP name | AP password | Dashboard login | IP |
|---|---------|-------------|-----------------|----|
| **Dashboard Arsenal** | `ArsenalNet` | `arsenal32` | `admin` / `arsenal` | `172.0.0.1` |
| **WebUI Bruce base** | `BruceNet` | `brucenet` | `admin` / `bruce` | `172.0.0.1` |

Tudo configurável em **Configurações** no dispositivo. Se um padrão não funciona, alguém mudou — regrave ou verifique Configurações → WiFi AP / WebUI. O IP é **`172.0.0.1`**, não `192.168.4.1`.

## Ferramentas (68)

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

✅ = funciona em Full e LITE. A maioria dos Full-only precisa de placa de 16 MB.

## Placas

**Full (16 MB):** T-Embed CC1101 · T-Deck / T-Deck Pro · T-Display S3 (all variants) · T-HMI · T-LoRa Pager · T-Watch S3 · M5Stack Cardputer / CoreS3 / StickS3 · ESP32-S3 DevKitC-1 · Smoochiee

**Full (4 MB):** ESP32-C5 / ESP32-C5 TFT

**LITE (4 MB):** M5Stack Core 4/16 MB · CPlus 1.1 · CYD-2432S028/2USB/W328C/W328R · CYD-3248S035R/C · Elecrow 2.4B/2.8B/3.5B · T-Display TTGO · Marauder Mini/V4-V6/V7/V6.1 · Awok Mini/Touch · WaveSentry R1 · Phantom S024R

**Problemas conhecidos:** M5Stack CPlus2 & Core2 — DRAM overflow, use LITE.

## Compilar do código

```bash
git clone https://github.com/quietdom/bruce-arsenal
cd bruce-arsenal
pio run -e <board-name>   # nomes em platformio.ini
```

## Créditos

- [Bruce Firmware](https://github.com/pr3y/Bruce) por pr3y — a base
- Módulo Arsenal + ferramentas por quietdom

## Aviso

Apenas para testes de segurança autorizados e fins educacionais. Não seja idiota com isso.
