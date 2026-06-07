🌐 [English](../README.md) | [Português](README_PT.md) | [Italiano](README_IT.md) | [Français](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Türkçe](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Español](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>Aangepaste offensieve beveiligingsfirmware voor ESP32-apparaten</b><br>
  <i>26 aanvalstools • Mobiel dashboard • Achtergrond-ontwijking • 60K+ scripts</i>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32--S3-blue?style=flat-square" alt="ESP32-S3">
  <img src="https://img.shields.io/badge/Target-T--Embed%20CC1101-green?style=flat-square" alt="T-Embed">
  <img src="https://img.shields.io/badge/Tools-26+-red?style=flat-square" alt="Tools">
  <img src="https://img.shields.io/badge/Scripts-60K+-purple?style=flat-square" alt="Scripts">
  <img src="https://img.shields.io/badge/Status-Active-brightgreen?style=flat-square" alt="Status">
</p>

---

## Wat Is Dit

Een zwaar gemodificeerde versie van [Bruce Firmware](https://github.com/pr3y/Bruce) met een aangepaste **Arsenal**-module — een compleet offensief beveiligingstoolkit toegankelijk vanuit één menutab met een vizierichoon.

Alle originele Bruce-functies blijven intact. Arsenal voegt 26 nieuwe tools toe, een mobiel webdashboard, achtergrond-ontwijkingsdiensten en een scriptbrowser die 60K+ payloads kan laden van de SD-kaart.

---

## Installatie

1. Download de `.bin` van [Releases](../../releases)
2. Flash naar je apparaat met een van de methoden:
   - **USB:** [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
   - **OTA:** Na de eerste flash, gebruik Arsenal → Dashboard → upload nieuwe .bin vanaf je telefoon
3. Klaar

---

## Arsenal Menu

```
🎯 Arsenal
├── WiFi Arsenal ──────── 8 offensieve WiFi-tools
├── BLE Arsenal ───────── 5 Bluetooth-aanvalstools
├── Inlichtingen ──────── 3 verkenning/bewustzijnstools
├── Ontwijking ────────── 5 stealth/anti-detectietools
├── Combo's ───────────── Vooraf ingestelde + aangepaste aanvalsketens
├── Planner ───────────── Tijdvertraagde aanvalsuitvoering
├── Scripts ───────────── Blader door 60K+ payloads van SD
├── Sessielog ─────────── Activiteitenregistratie op SD
├── Jam All ───────────── Gelijktijdige multi-band jamming (aangepaste GUI)
└── Dashboard ─────────── Mobiele web-UI + bestandsbeheer + OTA
```

---

## Tools

### WiFi Arsenal

| Tool | Wat Het Doet |
|------|--------------|
| **Network Scanner** | ARP-sweep + TCP-poortscan. Toont apparaten, open poorten, leveranciersnamen |
| **DHCP Starvation** | Overspoelt DHCP-verzoeken met vervalste MAC's om de IP-pool van de router uit te putten |
| **Karma Attack** | Beantwoordt alle WiFi probe-verzoeken — apparaten in de buurt verbinden automatisch met jou |
| **DNS Spoofer** | Onderschept DNS bij werking als AP. Alle domeinen verwijzen naar jouw portaal |
| **Portal Templates** | Laadt aangepaste phishing-HTML van SD-kaart. Vangt inloggegevens op naar bestand |
| **Auto-Phish Portal** | Detecteert welke netwerken slachtoffers zoeken, kloont het SSID, serveert portaal |
| **Cred Forward** | Vangt portaal-inloggegevens → verbindt automatisch met echte AP → brugt verkeer. Volledige transparante MITM |
| **WiFi Brute Force** | Genereert slimme woordenlijst van SSID-naam (jaarcombinaties, leet speak, veelvoorkomende patronen). Test live |

### BLE Arsenal

| Tool | Wat Het Doet |
|------|--------------|
| **BLE Tracker** | Continue BLE-scan met RSSI-afstandsschatting. Detecteert AirTags, SmartTags, Tiles die je volgen |
| **Name Spammer** | Overspoelt BLE-advertisingkanalen met honderden willekeurige apparaatnamen |
| **AirTag Spoofer** | Zendt valse Apple FindMy AirTag-beacons uit. Fantoomtags verschijnen op nabijgelegen iPhones |
| **Audio Jammer** | Verstoort BLE-audioverbindingen (AirPods, speakers) met advertisement-floods |
| **Notification Spoofer** | Pusht valse apparaat-gevonden-popups naar Android (Fast Pair), Windows (Swift Pair) en iOS (AirPods-nabijheid) |

### Inlichtingen

| Tool | Wat Het Doet |
|------|--------------|
| **Device Fingerprinter** | Passieve WiFi-fingerprinting. Identificeert apparaat-OS/model aan de hand van probe-gedrag, OUI, timing |
| **OPSEC Monitor** | Detecteert of je gescand, gedeauthenticeerd of gevolgd wordt. Dreigingsniveau: groen/geel/rood |
| **OUI Lookup** | Offline MAC-leveranciersdatabase-opzoeking van SD-kaart |

### Ontwijking

| Tool | Wat Het Doet |
|------|--------------|
| **MAC Rotator** | Randomiseert WiFi MAC-adres op configureerbare timer. Instelbare snelheid |
| **Channel Hopper** | Schakelt snel WiFi-kanalen tijdens operaties. Configureerbaar interval |
| **Decoy Traffic** | Zendt willekeurige beacon/probe-frames uit op roterende kanalen. Maskeert echte activiteit in ruis |
| **Identity Cloner** | Vangt identiteiten van nabije apparaten (MAC + probe-patroon). Kloon er een om onzichtbaar te worden |
| **QR Poisoner** | Genereert QR-codes die naar jouw portaal verwijzen. Toon op scherm, print en plaats |

### Jam All (Aangepaste GUI)

Volledig scherm dashboard met per-band toggles, live activiteitbalken, verstreken timer en one-press noodstop.

| Band | Methode |
|------|---------|
| WiFi 2.4GHz | Deauth-flood cyclend over alle 14 kanalen |
| BLE | Advertisement-kanaalflood (37, 38, 39) |
| Sub-GHz | CC1101 continue TX (315/433/868/915 MHz) |
| NRF24 | 2.4GHz-draaggolf op roterende kanalen |

### Hulpmiddelen

| Tool | Wat Het Doet |
|------|--------------|
| **Attack Scheduler** | Kies een tool → stel vertraging in (0 tot 1 uur) → stel duur in. Start en stopt automatisch |
| **Session Log** | Registreert elke actie met tijdstempels. Bekijk/exporteer/wis vanuit menu |
| **Script Browser** | Navigeer gecategoriseerde SD-mappen. Auto-lancering op basis van bestandstype (.txt/.sub/.ir/.html/.nfc/.js) |
| **Combo Presets** | Aanvalsketens met één tik. Ingebouwd: Stealth Mode, Full Attack, Passive Recon. Maak aangepaste .txt op SD |
| **Remote Dashboard** | Telefoon verbindt met ArsenalNet AP → volledige mobiele web-UI met bestandsbeheer + OTA-updates |

---

## Achtergronddiensten

Deze draaien persistent in alle menu's — niet alleen binnen Arsenal.

| Dienst | Beschrijving |
|--------|--------------|
| **OPSEC Dot** | Gekleurd bolletje in de statusbalk (groen/geel/rood). Altijd zichtbaar, elk scherm |
| **Always-On Evasion** | MAC-rotatie + channel hopping + decoy traffic. Draait stil op CPU-core 0 |
| **Low Power Mode** | Vertraagt achtergrondtaken 5x om batterij te sparen. Schakel vanuit Combo's menu |
| **Auto-Dim** | Schermhelderheid vermindert tijdens actieve aanvallen. Herstelt bij stoppen |

---

## Remote Dashboard

Start Arsenal → Dashboard. Verbind je telefoon met `ArsenalNet` (wachtwoord: `arsenal32`). Open `192.168.4.1` in de browser.

**Functies:**
- Start/stop alle 26 tools vanaf je telefoon
- Blader, upload, verwijder bestanden op SD-kaart (map `/arsenal/`)
- Upload `.bin`-firmware voor OTA-update
- Drag-and-drop scriptupload naar gecategoriseerde mappen
- Live systeemstatus (heap, SD-ruimte, uptime)

---

## SD-kaart Indeling

```
/arsenal/
├── badusb/       DuckyScript-payloads (.txt)
├── subghz/       Sub-GHz-signaalvangsten (.sub)
├── ir/           IR-afstandsbedieningsbestanden (.ir)
├── portals/      Evil portal HTML-sjablonen
├── nfc/          NFC-kaartdumps (.nfc)
├── rfid/         125kHz RFID-vangsten (.rfid)
├── ibutton/      iButton-sleutelbestanden (.ibutton)
├── scripts/      JavaScript-automatisering (.js)
├── combos/       Aangepaste combo-presets (.txt — één functienaam per regel)
├── logs/         Sessielogs (automatisch aangemaakt)
└── creds.txt     Gevangen inloggegevens (automatisch aangemaakt)
```

Compatibel met [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts) (60K+ scripts).

---

## Aangepaste Combo's

Maak een `.txt`-bestand in `/arsenal/combos/` met één functie per regel:

```
mac_rotator
channel_hopper
decoy_traffic
ble_tracker
```

Verschijnt automatisch in Arsenal → Combos.

---

## Hardware

Primair doel: **LilyGo T-Embed CC1101**
- ESP32-S3 (16MB flash, 8MB PSRAM)
- ST7789 320x170 TFT-display
- Rotary encoder + knop
- CC1101 Sub-GHz-radio
- NRF24 2.4GHz-radio
- PN532 NFC
- SD-kaartsleuf
- USB-C (BadUSB HID)
- Speaker + Microfoon
- RGB-LED's
- BQ27220 brandstofmeter

---

## Credits

- Gebaseerd op [Bruce Firmware](https://github.com/pr3y/Bruce) door pr3y
- Arsenal-module door quietdom
- Scriptcollectie compatibel met [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts)

---

## Disclaimer

Deze firmware is uitsluitend bedoeld voor geautoriseerd beveiligingstesten en educatieve doeleinden. Gebruik niet tegen netwerken of apparaten die je niet bezit of waarvoor je geen expliciete toestemming hebt om te testen. De auteurs zijn niet verantwoordelijk voor misbruik.
