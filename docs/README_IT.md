🌐 [English](../README.md) | [Português](README_PT.md) | [Italiano](README_IT.md) | [Français](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Türkçe](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Español](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>Firmware offensivo personalizzato per dispositivi ESP32</b><br>
  <i>26 strumenti di attacco • Dashboard mobile • Evasione in background • 60K+ script</i>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32--S3-blue?style=flat-square" alt="ESP32-S3">
  <img src="https://img.shields.io/badge/Target-T--Embed%20CC1101-green?style=flat-square" alt="T-Embed">
  <img src="https://img.shields.io/badge/Tools-26+-red?style=flat-square" alt="Tools">
  <img src="https://img.shields.io/badge/Scripts-60K+-purple?style=flat-square" alt="Scripts">
  <img src="https://img.shields.io/badge/Status-Active-brightgreen?style=flat-square" alt="Status">
</p>

---

## Cos'è Questo

Una versione pesantemente modificata di [Bruce Firmware](https://github.com/pr3y/Bruce) con un modulo **Arsenal** personalizzato integrato — un toolkit completo di sicurezza offensiva accessibile da una singola scheda menu con icona a mirino.

Tutte le funzionalità originali di Bruce rimangono intatte. Arsenal aggiunge 26 nuovi strumenti, una dashboard web mobile, servizi di evasione in background e un browser di script che può caricare oltre 60K payload dalla scheda SD.

---

## Installazione

1. Scarica il `.bin` da [Releases](../../releases)
2. Flasha sul tuo dispositivo usando uno dei metodi:
   - **USB:** [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
   - **OTA:** Dopo il primo flash, usa Arsenal → Dashboard → carica nuovo .bin dal telefono
3. Fatto

---

## Menu Arsenal

```
🎯 Arsenal
├── WiFi Arsenal ──────── 8 strumenti offensivi WiFi
├── BLE Arsenal ───────── 5 strumenti di attacco Bluetooth
├── Intelligence ──────── 3 strumenti di ricognizione/consapevolezza
├── Evasione ──────────── 5 strumenti stealth/anti-rilevamento
├── Combos ────────────── Catene di attacco predefinite + personalizzate
├── Scheduler ─────────── Esecuzione attacchi a tempo
├── Scripts ───────────── Sfoglia 60K+ payload da SD
├── Log Sessione ──────── Registrazione attività su SD
├── Jam All ───────────── Jamming simultaneo multi-banda (GUI personalizzata)
└── Dashboard ─────────── UI web mobile + file manager + OTA
```

---

## Strumenti

### WiFi Arsenal

| Strumento | Cosa Fa |
|-----------|---------|
| **Network Scanner** | Scansione ARP + scan porte TCP. Mostra dispositivi, porte aperte, nomi vendor |
| **DHCP Starvation** | Inonda richieste DHCP con MAC falsificati per esaurire il pool IP del router |
| **Karma Attack** | Risponde a tutte le richieste WiFi probe — i dispositivi vicini si connettono automaticamente a te |
| **DNS Spoofer** | Intercetta DNS quando funziona come AP. Tutti i domini risolvono al tuo portale |
| **Portal Templates** | Carica HTML di phishing personalizzato dalla scheda SD. Cattura credenziali su file |
| **Auto-Phish Portal** | Rileva quali reti cercano le vittime, clona il SSID, serve il portale |
| **Cred Forward** | Cattura credenziali del portale → si connette automaticamente all'AP reale → ponte del traffico. MITM trasparente completo |
| **WiFi Brute Force** | Genera wordlist intelligente dal nome SSID (combinazioni anno, leet speak, pattern comuni). Prova in tempo reale |

### BLE Arsenal

| Strumento | Cosa Fa |
|-----------|---------|
| **BLE Tracker** | Scansione BLE continua con stima distanza RSSI. Rileva AirTag, SmartTag, Tile che ti seguono |
| **Name Spammer** | Inonda i canali advertising BLE con centinaia di nomi dispositivo casuali |
| **AirTag Spoofer** | Trasmette beacon falsi Apple FindMy AirTag. Tag fantasma appaiono sugli iPhone vicini |
| **Audio Jammer** | Interrompe connessioni audio BLE (AirPods, speaker) con flood di advertisement |
| **Notification Spoofer** | Invia popup falsi di dispositivo trovato su Android (Fast Pair), Windows (Swift Pair) e iOS (prossimità AirPods) |

### Intelligence

| Strumento | Cosa Fa |
|-----------|---------|
| **Device Fingerprinter** | Fingerprinting WiFi passivo. Identifica SO/modello del dispositivo dal comportamento probe, OUI, timing |
| **OPSEC Monitor** | Rileva se sei scansionato, deautenticato o tracciato. Livello minaccia: verde/giallo/rosso |
| **OUI Lookup** | Ricerca offline database vendor MAC dalla scheda SD |

### Evasione

| Strumento | Cosa Fa |
|-----------|---------|
| **MAC Rotator** | Randomizza indirizzo MAC WiFi su timer configurabile. Velocità regolabile |
| **Channel Hopper** | Cambia rapidamente canali WiFi durante le operazioni. Intervallo configurabile |
| **Decoy Traffic** | Emette frame beacon/probe casuali su canali rotanti. Maschera attività reale nel rumore |
| **Identity Cloner** | Cattura identità dispositivi vicini (MAC + pattern probe). Clona uno per diventare invisibile |
| **QR Poisoner** | Genera QR code che puntano al tuo portale. Mostra sullo schermo, stampa e posiziona |

### Jam All (GUI Personalizzata)

Dashboard a schermo intero con toggle per banda, barre attività live, timer trascorso e pulsante di arresto immediato.

| Banda | Metodo |
|-------|--------|
| WiFi 2.4GHz | Flood deauth ciclando tutti i 14 canali |
| BLE | Flood canali advertisement (37, 38, 39) |
| Sub-GHz | TX continuo CC1101 (315/433/868/915 MHz) |
| NRF24 | Portante 2.4GHz su canali rotanti |

### Utilità

| Strumento | Cosa Fa |
|-----------|---------|
| **Attack Scheduler** | Scegli uno strumento → imposta ritardo (0 a 1 ora) → imposta durata. Si avvia e si ferma automaticamente |
| **Session Log** | Registra ogni azione con timestamp. Visualizza/esporta/cancella dal menu |
| **Script Browser** | Naviga cartelle SD categorizzate. Auto-avvia in base al tipo file (.txt/.sub/.ir/.html/.nfc/.js) |
| **Combo Presets** | Catene di attacco con un tocco. Inclusi: Stealth Mode, Full Attack, Passive Recon. Crea personalizzati in .txt su SD |
| **Remote Dashboard** | Telefono si connette all'AP ArsenalNet → UI web mobile completa con file manager + aggiornamenti OTA |

---

## Servizi in Background

Questi funzionano persistentemente in tutti i menu — non solo dentro Arsenal.

| Servizio | Descrizione |
|----------|-------------|
| **OPSEC Dot** | Cerchio colorato nella barra di stato (verde/giallo/rosso). Sempre visibile, ogni schermata |
| **Always-On Evasion** | Rotazione MAC + channel hopping + decoy traffic. Gira silenziosamente sul core 0 della CPU |
| **Low Power Mode** | Rallenta i task in background 5x per risparmiare batteria. Attiva dal menu Combos |
| **Auto-Dim** | Luminosità schermo si riduce durante attacchi attivi. Si ripristina allo stop |

---

## Remote Dashboard

Avvia Arsenal → Dashboard. Connetti il telefono a `ArsenalNet` (password: `arsenal32`). Apri `192.168.4.1` nel browser.

**Funzionalità:**
- Avvia/ferma tutti i 26 strumenti dal telefono
- Sfoglia, carica, elimina file sulla scheda SD (cartella `/arsenal/`)
- Carica firmware `.bin` per aggiornamento OTA
- Upload script con drag-and-drop nelle cartelle categorizzate
- Stato sistema live (heap, spazio SD, uptime)

---

## Layout Scheda SD

```
/arsenal/
├── badusb/       Payload DuckyScript (.txt)
├── subghz/       Catture segnali Sub-GHz (.sub)
├── ir/           File telecomando IR (.ir)
├── portals/      Template HTML evil portal
├── nfc/          Dump schede NFC (.nfc)
├── rfid/         Catture RFID 125kHz (.rfid)
├── ibutton/      File chiave iButton (.ibutton)
├── scripts/      Automazione JavaScript (.js)
├── combos/       Preset combo personalizzati (.txt — un nome feature per riga)
├── logs/         Log sessione (creato automaticamente)
└── creds.txt     Credenziali catturate (creato automaticamente)
```

Compatibile con [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts) (60K+ script).

---

## Combo Personalizzati

Crea un file `.txt` in `/arsenal/combos/` con una feature per riga:

```
mac_rotator
channel_hopper
decoy_traffic
ble_tracker
```

Appare automaticamente in Arsenal → Combos.

---

## Hardware

Target principale: **LilyGo T-Embed CC1101**
- ESP32-S3 (16MB flash, 8MB PSRAM)
- Display TFT ST7789 320x170
- Encoder rotativo + pulsante
- Radio Sub-GHz CC1101
- Radio NRF24 2.4GHz
- NFC PN532
- Slot scheda SD
- USB-C (BadUSB HID)
- Altoparlante + Microfono
- LED RGB
- Fuel gauge BQ27220

---

## Crediti

- Basato su [Bruce Firmware](https://github.com/pr3y/Bruce) di pr3y
- Modulo Arsenal di quietdom
- Collezione script compatibile con [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts)

---

## Disclaimer

Questo firmware è destinato esclusivamente a test di sicurezza autorizzati e scopi educativi. Non utilizzare contro reti o dispositivi che non possiedi o per i quali non hai esplicita autorizzazione a testare. Gli autori non sono responsabili per l'uso improprio.
