🌐 [English](../README.md) | [Português](README_PT.md) | [Italiano](README_IT.md) | [Français](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Türkçe](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Español](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>Benutzerdefinierte offensive Sicherheits-Firmware für ESP32-Geräte</b><br>
  <i>26 Angriffswerkzeuge • Mobiles Dashboard • Hintergrund-Evasion • 60K+ Skripte</i>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32--S3-blue?style=flat-square" alt="ESP32-S3">
  <img src="https://img.shields.io/badge/Target-T--Embed%20CC1101-green?style=flat-square" alt="T-Embed">
  <img src="https://img.shields.io/badge/Tools-26+-red?style=flat-square" alt="Tools">
  <img src="https://img.shields.io/badge/Scripts-60K+-purple?style=flat-square" alt="Scripts">
  <img src="https://img.shields.io/badge/Status-Active-brightgreen?style=flat-square" alt="Status">
</p>

---

## Was Ist Das

Eine stark modifizierte Version von [Bruce Firmware](https://github.com/pr3y/Bruce) mit einem benutzerdefinierten **Arsenal**-Modul — ein vollständiges offensives Sicherheits-Toolkit, zugänglich über einen einzelnen Menü-Tab mit Fadenkreuz-Symbol.

Alle ursprünglichen Bruce-Funktionen bleiben erhalten. Arsenal fügt 26 neue Werkzeuge hinzu, ein mobiles Web-Dashboard, Hintergrund-Evasions-Dienste und einen Skript-Browser, der 60K+ Payloads von der SD-Karte laden kann.

---

## Installation

1. Laden Sie die `.bin` von [Releases](../../releases) herunter
2. Flashen Sie auf Ihr Gerät mit einer der Methoden:
   - **USB:** [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
   - **OTA:** Nach dem ersten Flash, nutzen Sie Arsenal → Dashboard → neue .bin vom Handy hochladen
3. Fertig

---

## Arsenal Menü

```
🎯 Arsenal
├── WiFi Arsenal ──────── 8 offensive WiFi-Werkzeuge
├── BLE Arsenal ───────── 5 Bluetooth-Angriffswerkzeuge
├── Aufklärung ────────── 3 Erkennungs-/Bewusstseinstools
├── Evasion ───────────── 5 Stealth-/Anti-Erkennungswerkzeuge
├── Combos ────────────── Vordefinierte + benutzerdefinierte Angriffsketten
├── Planer ────────────── Zeitverzögerte Angriffsausführung
├── Skripte ───────────── 60K+ Payloads von SD durchsuchen
├── Sitzungslog ────────── Aktivitätsprotokollierung auf SD
├── Jam All ───────────── Gleichzeitiges Multi-Band-Jamming (benutzerdefinierte GUI)
└── Dashboard ─────────── Mobile Web-UI + Dateimanager + OTA
```

---

## Werkzeuge

### WiFi Arsenal

| Werkzeug | Was Es Macht |
|----------|--------------|
| **Network Scanner** | ARP-Sweep + TCP-Port-Scan. Zeigt Geräte, offene Ports, Herstellernamen |
| **DHCP Starvation** | Flutet DHCP-Anfragen mit gefälschten MACs um den IP-Pool des Routers zu erschöpfen |
| **Karma Attack** | Beantwortet alle WiFi Probe-Anfragen — Geräte in der Nähe verbinden sich automatisch mit Ihnen |
| **DNS Spoofer** | Fängt DNS ab bei Betrieb als AP. Alle Domains verweisen auf Ihr Portal |
| **Portal Templates** | Lädt benutzerdefiniertes Phishing-HTML von SD-Karte. Erfasst Anmeldedaten in Datei |
| **Auto-Phish Portal** | Erkennt welche Netzwerke Opfer suchen, klont die SSID, liefert Portal |
| **Cred Forward** | Erfasst Portal-Anmeldedaten → verbindet automatisch mit echtem AP → überbrückt Traffic. Vollständiger transparenter MITM |
| **WiFi Brute Force** | Generiert intelligente Wortliste aus SSID-Name (Jahreskombinationen, Leet Speak, häufige Muster). Testet live |

### BLE Arsenal

| Werkzeug | Was Es Macht |
|----------|--------------|
| **BLE Tracker** | Kontinuierlicher BLE-Scan mit RSSI-Distanzschätzung. Erkennt AirTags, SmartTags, Tiles die Ihnen folgen |
| **Name Spammer** | Flutet BLE-Advertising-Kanäle mit Hunderten zufälliger Gerätenamen |
| **AirTag Spoofer** | Sendet gefälschte Apple FindMy AirTag-Beacons. Phantom-Tags erscheinen auf nahegelegenen iPhones |
| **Audio Jammer** | Stört BLE-Audioverbindungen (AirPods, Lautsprecher) mit Advertisement-Floods |
| **Notification Spoofer** | Sendet gefälschte Gerät-gefunden-Popups an Android (Fast Pair), Windows (Swift Pair) und iOS (AirPods-Nähe) |

### Aufklärung

| Werkzeug | Was Es Macht |
|----------|--------------|
| **Device Fingerprinter** | Passives WiFi-Fingerprinting. Identifiziert Geräte-OS/Modell anhand von Probe-Verhalten, OUI, Timing |
| **OPSEC Monitor** | Erkennt ob Sie gescannt, deauthentifiziert oder verfolgt werden. Bedrohungsstufe: grün/gelb/rot |
| **OUI Lookup** | Offline MAC-Hersteller-Datenbanksuche von SD-Karte |

### Evasion

| Werkzeug | Was Es Macht |
|----------|--------------|
| **MAC Rotator** | Randomisiert WiFi MAC-Adresse auf konfigurierbarem Timer. Einstellbare Geschwindigkeit |
| **Channel Hopper** | Wechselt schnell WiFi-Kanäle während Operationen. Konfigurierbares Intervall |
| **Decoy Traffic** | Sendet zufällige Beacon/Probe-Frames auf rotierenden Kanälen. Maskiert echte Aktivität im Rauschen |
| **Identity Cloner** | Erfasst Identitäten nahegelegener Geräte (MAC + Probe-Muster). Klonen Sie eines um unsichtbar zu werden |
| **QR Poisoner** | Generiert QR-Codes die auf Ihr Portal verweisen. Auf Bildschirm anzeigen, drucken und platzieren |

### Jam All (Benutzerdefinierte GUI)

Vollbild-Dashboard mit Per-Band-Umschaltern, Live-Aktivitätsbalken, verstrichener Timer und Ein-Knopf-Notstopp.

| Band | Methode |
|------|---------|
| WiFi 2.4GHz | Deauth-Flood über alle 14 Kanäle zyklisch |
| BLE | Advertisement-Kanal-Flood (37, 38, 39) |
| Sub-GHz | CC1101 kontinuierliche Sendung (315/433/868/915 MHz) |
| NRF24 | 2.4GHz-Träger auf rotierenden Kanälen |

### Dienstprogramme

| Werkzeug | Was Es Macht |
|----------|--------------|
| **Attack Scheduler** | Werkzeug wählen → Verzögerung einstellen (0 bis 1 Stunde) → Dauer einstellen. Startet und stoppt automatisch |
| **Session Log** | Protokolliert jede Aktion mit Zeitstempel. Anzeigen/Exportieren/Löschen aus Menü |
| **Script Browser** | Kategorisierte SD-Ordner durchsuchen. Auto-Start basierend auf Dateityp (.txt/.sub/.ir/.html/.nfc/.js) |
| **Combo Presets** | Angriffsketten mit einem Tipp. Eingebaut: Stealth Mode, Full Attack, Passive Recon. Eigene .txt auf SD erstellen |
| **Remote Dashboard** | Handy verbindet mit ArsenalNet AP → vollständige mobile Web-UI mit Dateimanager + OTA-Updates |

---

## Hintergrunddienste

Diese laufen persistent in allen Menüs — nicht nur innerhalb von Arsenal.

| Dienst | Beschreibung |
|--------|--------------|
| **OPSEC Dot** | Farbiger Kreis in der Statusleiste (grün/gelb/rot). Immer sichtbar, auf jedem Bildschirm |
| **Always-On Evasion** | MAC-Rotation + Channel Hopping + Decoy Traffic. Läuft lautlos auf CPU-Kern 0 |
| **Low Power Mode** | Verlangsamt Hintergrundaufgaben 5x um Akku zu sparen. Umschalten im Combos-Menü |
| **Auto-Dim** | Bildschirmhelligkeit sinkt während aktiver Angriffe. Stellt sich bei Stopp wieder her |

---

## Remote Dashboard

Starten Sie Arsenal → Dashboard. Verbinden Sie Ihr Handy mit `ArsenalNet` (Passwort: `arsenal32`). Öffnen Sie `192.168.4.1` im Browser.

**Funktionen:**
- Alle 26 Werkzeuge vom Handy starten/stoppen
- Dateien auf SD-Karte durchsuchen, hochladen, löschen (Ordner `/arsenal/`)
- `.bin`-Firmware für OTA-Update hochladen
- Drag-and-Drop Skript-Upload in kategorisierte Ordner
- Live-Systemstatus (Heap, SD-Speicher, Laufzeit)

---

## SD-Karten-Layout

```
/arsenal/
├── badusb/       DuckyScript-Payloads (.txt)
├── subghz/       Sub-GHz-Signalerfassungen (.sub)
├── ir/           IR-Fernbedienungsdateien (.ir)
├── portals/      Evil Portal HTML-Vorlagen
├── nfc/          NFC-Karten-Dumps (.nfc)
├── rfid/         125kHz RFID-Erfassungen (.rfid)
├── ibutton/      iButton-Schlüsseldateien (.ibutton)
├── scripts/      JavaScript-Automatisierung (.js)
├── combos/       Benutzerdefinierte Combo-Voreinstellungen (.txt — ein Feature-Name pro Zeile)
├── logs/         Sitzungsprotokolle (automatisch erstellt)
└── creds.txt     Erfasste Anmeldedaten (automatisch erstellt)
```

Kompatibel mit [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts) (60K+ Skripte).

---

## Benutzerdefinierte Combos

Erstellen Sie eine `.txt`-Datei in `/arsenal/combos/` mit einem Feature pro Zeile:

```
mac_rotator
channel_hopper
decoy_traffic
ble_tracker
```

Erscheint automatisch in Arsenal → Combos.

---

## Hardware

Primäres Ziel: **LilyGo T-Embed CC1101**
- ESP32-S3 (16MB Flash, 8MB PSRAM)
- ST7789 320x170 TFT-Display
- Drehencoder + Taste
- CC1101 Sub-GHz-Funkmodul
- NRF24 2.4GHz-Funkmodul
- PN532 NFC
- SD-Karten-Slot
- USB-C (BadUSB HID)
- Lautsprecher + Mikrofon
- RGB-LEDs
- BQ27220 Ladezustandsanzeige

---

## Danksagungen

- Basierend auf [Bruce Firmware](https://github.com/pr3y/Bruce) von pr3y
- Arsenal-Modul von quietdom
- Skript-Sammlung kompatibel mit [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts)

---

## Haftungsausschluss

Diese Firmware ist ausschließlich für autorisierte Sicherheitstests und Bildungszwecke bestimmt. Verwenden Sie sie nicht gegen Netzwerke oder Geräte, die Sie nicht besitzen oder für deren Test Sie keine ausdrückliche Genehmigung haben. Die Autoren sind nicht verantwortlich für Missbrauch.
