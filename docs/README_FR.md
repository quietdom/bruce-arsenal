🌐 [English](../README.md) | [Português](README_PT.md) | [Italiano](README_IT.md) | [Français](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Türkçe](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Español](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>Firmware offensif personnalisé pour appareils ESP32</b><br>
  <i>26 outils d'attaque • Tableau de bord mobile • Évasion en arrière-plan • 60K+ scripts</i>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32--S3-blue?style=flat-square" alt="ESP32-S3">
  <img src="https://img.shields.io/badge/Target-T--Embed%20CC1101-green?style=flat-square" alt="T-Embed">
  <img src="https://img.shields.io/badge/Tools-26+-red?style=flat-square" alt="Tools">
  <img src="https://img.shields.io/badge/Scripts-60K+-purple?style=flat-square" alt="Scripts">
  <img src="https://img.shields.io/badge/Status-Active-brightgreen?style=flat-square" alt="Status">
</p>

---

## Qu'est-ce Que C'est

Une version lourdement modifiée de [Bruce Firmware](https://github.com/pr3y/Bruce) avec un module **Arsenal** personnalisé intégré — un kit complet de sécurité offensive accessible depuis un seul onglet de menu avec une icône de viseur.

Toutes les fonctionnalités originales de Bruce restent intactes. Arsenal ajoute 26 nouveaux outils, un tableau de bord web mobile, des services d'évasion en arrière-plan et un navigateur de scripts pouvant charger plus de 60K payloads depuis la carte SD.

---

## Installation

1. Téléchargez le `.bin` depuis [Releases](../../releases)
2. Flashez sur votre appareil en utilisant l'une des méthodes :
   - **USB :** [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
   - **OTA :** Après le premier flash, utilisez Arsenal → Dashboard → téléversez un nouveau .bin depuis le téléphone
3. Terminé

---

## Menu Arsenal

```
🎯 Arsenal
├── WiFi Arsenal ──────── 8 outils offensifs WiFi
├── BLE Arsenal ───────── 5 outils d'attaque Bluetooth
├── Intelligence ──────── 3 outils de reconnaissance/vigilance
├── Évasion ───────────── 5 outils furtivité/anti-détection
├── Combos ────────────── Chaînes d'attaque prédéfinies + personnalisées
├── Scheduler ─────────── Exécution d'attaques à retardement
├── Scripts ───────────── Parcourez 60K+ payloads depuis SD
├── Log de Session ────── Journalisation d'activité sur SD
├── Jam All ───────────── Brouillage simultané multi-bande (GUI personnalisée)
└── Dashboard ─────────── UI web mobile + gestionnaire de fichiers + OTA
```

---

## Outils

### WiFi Arsenal

| Outil | Ce Qu'il Fait |
|-------|---------------|
| **Network Scanner** | Balayage ARP + scan de ports TCP. Affiche les appareils, ports ouverts, noms de fabricants |
| **DHCP Starvation** | Inonde de requêtes DHCP avec des MAC falsifiées pour épuiser le pool IP du routeur |
| **Karma Attack** | Répond à toutes les requêtes WiFi probe — les appareils à proximité se connectent automatiquement à vous |
| **DNS Spoofer** | Intercepte le DNS en mode AP. Tous les domaines résolvent vers votre portail |
| **Portal Templates** | Charge du HTML de phishing personnalisé depuis la carte SD. Capture les identifiants dans un fichier |
| **Auto-Phish Portal** | Détecte les réseaux que les victimes recherchent, clone le SSID, sert le portail |
| **Cred Forward** | Capture les identifiants du portail → se connecte automatiquement au vrai AP → fait le pont du trafic. MITM transparent complet |
| **WiFi Brute Force** | Génère une wordlist intelligente à partir du nom SSID (combinaisons d'années, leet speak, motifs courants). Teste en temps réel |

### BLE Arsenal

| Outil | Ce Qu'il Fait |
|-------|---------------|
| **BLE Tracker** | Scan BLE continu avec estimation de distance RSSI. Détecte les AirTag, SmartTag, Tile qui vous suivent |
| **Name Spammer** | Inonde les canaux advertising BLE avec des centaines de noms d'appareils aléatoires |
| **AirTag Spoofer** | Diffuse de faux beacons Apple FindMy AirTag. Des tags fantômes apparaissent sur les iPhone à proximité |
| **Audio Jammer** | Perturbe les connexions audio BLE (AirPods, enceintes) avec un flood d'advertisements |
| **Notification Spoofer** | Envoie de faux popups d'appareil trouvé sur Android (Fast Pair), Windows (Swift Pair) et iOS (proximité AirPods) |

### Intelligence

| Outil | Ce Qu'il Fait |
|-------|---------------|
| **Device Fingerprinter** | Fingerprinting WiFi passif. Identifie l'OS/modèle de l'appareil par le comportement probe, OUI, timing |
| **OPSEC Monitor** | Détecte si vous êtes scanné, désauthentifié ou traqué. Niveau de menace : vert/jaune/rouge |
| **OUI Lookup** | Recherche hors ligne dans la base de données des fabricants MAC depuis la carte SD |

### Évasion

| Outil | Ce Qu'il Fait |
|-------|---------------|
| **MAC Rotator** | Randomise l'adresse MAC WiFi sur un minuteur configurable. Vitesse ajustable |
| **Channel Hopper** | Change rapidement de canal WiFi pendant les opérations. Intervalle configurable |
| **Decoy Traffic** | Émet des frames beacon/probe aléatoires sur des canaux rotatifs. Masque l'activité réelle dans le bruit |
| **Identity Cloner** | Capture les identités des appareils à proximité (MAC + modèle de probe). Clonez-en un pour devenir invisible |
| **QR Poisoner** | Génère des QR codes pointant vers votre portail. Affichez sur l'écran, imprimez et placez |

### Jam All (GUI Personnalisée)

Tableau de bord plein écran avec toggles par bande, barres d'activité en direct, chronomètre écoulé et bouton d'arrêt d'urgence.

| Bande | Méthode |
|-------|---------|
| WiFi 2.4GHz | Flood de deauth cyclant les 14 canaux |
| BLE | Flood des canaux advertisement (37, 38, 39) |
| Sub-GHz | TX continu CC1101 (315/433/868/915 MHz) |
| NRF24 | Porteuse 2.4GHz sur canaux rotatifs |

### Utilitaires

| Outil | Ce Qu'il Fait |
|-------|---------------|
| **Attack Scheduler** | Choisissez un outil → définissez le délai (0 à 1 heure) → définissez la durée. Démarre et s'arrête automatiquement |
| **Session Log** | Enregistre chaque action avec horodatage. Consultez/exportez/effacez depuis le menu |
| **Script Browser** | Naviguez dans les dossiers SD catégorisés. Auto-lance selon le type de fichier (.txt/.sub/.ir/.html/.nfc/.js) |
| **Combo Presets** | Chaînes d'attaque en un toucher. Inclus : Stealth Mode, Full Attack, Passive Recon. Créez des personnalisés en .txt sur SD |
| **Remote Dashboard** | Le téléphone se connecte à l'AP ArsenalNet → UI web mobile complète avec gestionnaire de fichiers + mises à jour OTA |

---

## Services en Arrière-plan

Ceux-ci fonctionnent de manière persistante dans tous les menus — pas seulement dans Arsenal.

| Service | Description |
|---------|-------------|
| **OPSEC Dot** | Cercle coloré dans la barre d'état (vert/jaune/rouge). Toujours visible, sur chaque écran |
| **Always-On Evasion** | Rotation MAC + channel hopping + decoy traffic. Tourne silencieusement sur le core 0 du CPU |
| **Low Power Mode** | Ralentit les tâches en arrière-plan 5x pour économiser la batterie. Activez depuis le menu Combos |
| **Auto-Dim** | La luminosité de l'écran diminue pendant les attaques actives. Se rétablit à l'arrêt |

---

## Remote Dashboard

Lancez Arsenal → Dashboard. Connectez votre téléphone à `ArsenalNet` (mot de passe : `arsenal32`). Ouvrez `192.168.4.1` dans le navigateur.

**Fonctionnalités :**
- Démarrez/arrêtez les 26 outils depuis le téléphone
- Parcourez, téléversez, supprimez des fichiers sur la carte SD (dossier `/arsenal/`)
- Téléversez un firmware `.bin` pour mise à jour OTA
- Upload de scripts par glisser-déposer dans les dossiers catégorisés
- État système en direct (heap, espace SD, uptime)

---

## Organisation de la Carte SD

```
/arsenal/
├── badusb/       Payloads DuckyScript (.txt)
├── subghz/       Captures de signaux Sub-GHz (.sub)
├── ir/           Fichiers télécommande IR (.ir)
├── portals/      Templates HTML evil portal
├── nfc/          Dumps de cartes NFC (.nfc)
├── rfid/         Captures RFID 125kHz (.rfid)
├── ibutton/      Fichiers clé iButton (.ibutton)
├── scripts/      Automatisation JavaScript (.js)
├── combos/       Presets combo personnalisés (.txt — un nom de feature par ligne)
├── logs/         Logs de session (créé automatiquement)
└── creds.txt     Identifiants capturés (créé automatiquement)
```

Compatible avec [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts) (60K+ scripts).

---

## Combos Personnalisés

Créez un fichier `.txt` dans `/arsenal/combos/` avec une feature par ligne :

```
mac_rotator
channel_hopper
decoy_traffic
ble_tracker
```

Apparaît automatiquement dans Arsenal → Combos.

---

## Hardware

Cible principale : **LilyGo T-Embed CC1101**
- ESP32-S3 (16MB flash, 8MB PSRAM)
- Écran TFT ST7789 320x170
- Encodeur rotatif + bouton
- Radio Sub-GHz CC1101
- Radio NRF24 2.4GHz
- NFC PN532
- Slot carte SD
- USB-C (BadUSB HID)
- Haut-parleur + Microphone
- LEDs RGB
- Jauge de batterie BQ27220

---

## Crédits

- Basé sur [Bruce Firmware](https://github.com/pr3y/Bruce) par pr3y
- Module Arsenal par quietdom
- Collection de scripts compatible avec [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts)

---

## Avertissement

Ce firmware est destiné exclusivement aux tests de sécurité autorisés et à des fins éducatives. Ne pas utiliser contre des réseaux ou appareils que vous ne possédez pas ou pour lesquels vous n'avez pas l'autorisation explicite de tester. Les auteurs ne sont pas responsables d'une utilisation abusive.
