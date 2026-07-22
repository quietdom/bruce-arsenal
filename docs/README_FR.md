🌐 [English](../README.md) | [Portugues](README_PT.md) | [Italiano](README_IT.md) | **Francais** | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Turkce](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Espanol](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>Firmware de sécurité offensive personnalisé pour appareils ESP32</b><br>
  <i>68 outils — un firmware, de nombreuses cartes</i>
</p>

---

Une version modifiée du [Bruce Firmware](https://github.com/pr3y/Bruce) avec un module **Arsenal** personnalisé. Toutes les fonctions Bruce d'origine restent intactes. Compilé et testé sur 30 cartes avant chaque release.

## Nouveautés v1.2.0

**Corrigé:**
- Freeze RF Listen sur T-Embed — corrigé (bus SPI partagé libéré en sortie)
- « CC1101 introuvable » — corrigé
- NRF24 « fonctionne à peine » — corrigé (vérifie la puce et l'alimente)
- jam_all — les 4 bandes corrigées
- Compteurs flood/spam — affichent maintenant envoyés/échoués réels
- WireGuard — la clé privée n'est plus imprimée sur le port série
- Deauther — impl de retour dans le .cpp, linkage corrigé

**Nouveau:**
- **Universal IR Remote** — télécommande TV par marque, codes en flash, sans SD
- **PMKID Capture** — WPA sans client, sauve HC22000 sur SD
- **Deauth Detector** — défensif, alerte quand on vous déauthente
- **Rolljam** — capture les codes via jam + RX
- **Real MouseJack** — exécute maintenant l'implémentation réelle de 934 lignes
- **Saved target lists** — sauve/charge les cibles deauth sur SD
- **Refreshed BLE spammer** — 40 FastPair ID réels + 18 modèles Apple

## Installation

1. Téléchargez le `.bin` de votre carte depuis [Releases](../../releases)
2. Ouvrez l'[ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome ou Edge)
3. **Connect**, choisissez le port série ESP32
4. **Program**, sélectionnez le `.bin`, adresse `0x0`
5. Programmez, attendez, appuyez sur **RST** ou rebranchez l'USB

---

## Outils Arsenal (68)

| # | Tool | Full | LITE | | # | Tool | Full | LITE |
|---|------|:----:|:----:|-|---|------|:----:|:----:|
| 1 | Network Scanner | ✅ | ✅ | 35 | MAC Rotator | ✅ | ✅ |
| 2 | DHCP Starvation | ✅ | ✅ | 36 | Channel Hopper | ✅ | ✅ |
| 3 | Karma Attack | ✅ | ✅ | 37 | Keyfob Logger | ✅ | ❌ |
| 4 | DNS Spoofer | ✅ | ✅ | 38 | Frequency Scanner | ✅ | ✅ |
| 5 | Auto-Phish Portal | ✅ | ❌ | 39 | Flipper Import | ✅ | ✅ |
| 6 | Cred Forward | ✅ | ❌ | 40 | Flipper Detector | ✅ | ❌ |
| 7 | Auth Flood | ✅ | ✅ | 41 | Hacker Detector | ✅ | ❌ |
| 8 | AP Clone Flood | ✅ | ✅ | 42 | RF Silence Enforcer | ✅ | ❌ |
| 9 | SSL Strip Lite | ✅ | ❌ | 43 | ESP-NOW Chat | ✅ | ❌ |
| 10 | DNS Tunnel | ✅ | ✅ | 44 | ESP-NOW C2 | ✅ | ❌ |
| 11 | WPS PIN Reference | ✅ | ✅ | 45 | Dead Drop Mesh | ✅ | ❌ |
| 12 | UPnP Port Opener | ✅ | ❌ | 46 | IR Data Transfer | ✅ | ❌ |
| 13 | Default Cred Scanner | ✅ | ❌ | 47 | Multi-Device Sync | ✅ | ❌ |
| 14 | Rogue AP Detector | ✅ | ✅ | 48 | NFC Biz Card | ✅ | ❌ |
| 15 | WiFi Bruteforce | ✅ | ❌ | 49 | Attack Stats | ✅ | ✅ |
| 16 | WPA Handshake Grabber | ✅ | ❌ | 50 | Password Generator | ✅ | ✅ |
| 17 | Beacon Flood | ✅ | ❌ | 51 | Jam All | ✅ | ✅ |
| 18 | Selective Deauth | ✅ | ❌ | 52 | Combos | ✅ | ✅ |
| 19 | Enhanced Deauth | ✅ | ❌ | 53 | Scheduler | ✅ | ✅ |
| 20 | ARP Poisoner | ✅ | ❌ | 54 | Scripts | ✅ | ✅ |
| 21 | BLE Tracker | ✅ | ❌ | 55 | Session Log | ✅ | ✅ |
| 22 | BT Name Spammer | ✅ | ❌ | 56 | Config AP | ✅ | ✅ |
| 23 | AirTag Spoofer | ✅ | ❌ | 57 | Config Dashboard | ✅ | ✅ |
| 24 | Audio Jammer | ✅ | ❌ | 58 | PIN Lock | ✅ | ✅ |
| 25 | Notification Spoofer | ✅ | ❌ | 59 | Remote Dashboard | ✅ | ❌ |
| 26 | BT Rickroll | ✅ | ❌ | 60 | SSID History Logger | ✅ | ✅ |
| 27 | BT Device Profiler | ✅ | ❌ | 61 | QR Poisoner | ✅ | ✅ |
| 28 | Device Fingerprinter | ✅ | ✅ | 62 | Auto-Dim | ✅ | ✅ |
| 29 | OPSEC Monitor | ✅ | ✅ | 63 | OPSEC Background | ✅ | ✅ |
| 30 | OUI Lookup | ✅ | ✅ | 64 | Deauth Detector | ✅ | ❌ |
| 31 | Probe Log | ✅ | ✅ | 65 | PMKID Capture | ✅ | ❌ |
| 32 | Banner Grabber | ✅ | ✅ | 66 | Rolljam | ✅ | ❌ |
| 33 | SmartHome Scan | ✅ | ❌ | 67 | NRF24 MouseJack | ✅ | ❌ |
| 34 | Channel Chart | ✅ | ✅ | 68 | Universal IR Remote | ✅ | ✅ |

---

## Compatibilité des cartes

| **ESP32-S3** | | |
| LilyGo T-Embed CC1101 | 16 MB | Full |
| LilyGo T-Deck Pro | 16 MB | Full |
| LilyGo T-Deck | 16 MB | Full |
| LilyGo T-Display S3 (all variants) | 16 MB | Full |
| LilyGo T-HMI | 16 MB | Full |
| LilyGo T-LoRa Pager | 16 MB | Full |
| LilyGo T-Watch S3 | 16 MB | Full |
| M5Stack Cardputer | 8 MB | Full |
| M5Stack CoreS3 | 16 MB | Full |
| M5Stack StickS3 | 16 MB | Full |
| ESP32-S3 DevKitC-1 | 16 MB | Full |
| Smoochiee Board | 16 MB | Full |
| **ESP32-C5** | | |
| ESP32-C5 | 4 MB | Full |
| ESP32-C5 TFT | 4 MB | Full |
| **ESP32 (4 MB, LITE)** | | |
| M5Stack Core 4 MB | 4 MB | LITE |
| M5Stack Core 16 MB | 16 MB | LITE |
| M5Stack CPlus 1.1 | 4 MB | LITE |
| CYD-2432S028 / 2USB / W328C / W328R | 4 MB | LITE |
| CYD-3248S035R / S035C | 4 MB | LITE |
| Elecrow 2.4B / 2.8B / 3.5B | 4 MB | LITE |
| LilyGo T-Display TTGO | 4 MB | LITE |
| Marauder Mini / V4-V6 / V7 / V6.1 | 4 MB | LITE |
| Awok Mini / Touch | 4 MB | LITE |
| WaveSentry R1 / Phantom S024R | 4 MB | LITE |
| **Problèmes connus** | | |
| M5Stack CPlus2 | — | DRAM overflow — use LITE |
| M5Stack Core2 | — | DRAM overflow — use LITE |

---

## Tableau de bord distant

Arsenal > Dashboard. Connectez-vous à `BruceNet` (mot de passe : `bruce32`). Ouvrez `192.168.4.1`.

## Crédits

- [Bruce Firmware](https://github.com/pr3y/Bruce) par pr3y — la base de ce projet
- Module Arsenal et outils personnalisés par quietdom

## Avertissement

Pour tests de sécurité autorisés et usages éducatifs uniquement. Ne soyez pas idiot avec ça.
