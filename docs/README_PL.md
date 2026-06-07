🌐 [English](../README.md) | [Português](README_PT.md) | [Italiano](README_IT.md) | [Français](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Türkçe](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Español](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>Niestandardowy firmware ofensywny dla urządzeń ESP32</b><br>
  <i>26 narzędzi ataku • Mobilny panel • Unikanie w tle • 60K+ skryptów</i>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32--S3-blue?style=flat-square" alt="ESP32-S3">
  <img src="https://img.shields.io/badge/Target-T--Embed%20CC1101-green?style=flat-square" alt="T-Embed">
  <img src="https://img.shields.io/badge/Tools-26+-red?style=flat-square" alt="Tools">
  <img src="https://img.shields.io/badge/Scripts-60K+-purple?style=flat-square" alt="Scripts">
  <img src="https://img.shields.io/badge/Status-Active-brightgreen?style=flat-square" alt="Status">
</p>

---

## Co To Jest

Głęboko zmodyfikowana wersja [Bruce Firmware](https://github.com/pr3y/Bruce) z niestandardowym modułem **Arsenal** — kompletnym zestawem narzędzi bezpieczeństwa ofensywnego dostępnym z jednej zakładki menu z ikoną celownika.

Wszystkie oryginalne funkcje Bruce pozostają nienaruszone. Arsenal dodaje 26 nowych narzędzi, mobilny panel webowy, usługi unikania w tle oraz przeglądarkę skryptów mogącą załadować ponad 60K payloadów z karty SD.

---

## Instalacja

1. Pobierz `.bin` z [Releases](../../releases)
2. Wgraj na urządzenie jedną z metod:
   - **USB:** [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
   - **OTA:** Po pierwszym flashowaniu użyj Arsenal → Dashboard → wgraj nowy .bin z telefonu
3. Gotowe

---

## Menu Arsenal

```
🎯 Arsenal
├── WiFi Arsenal ──────── 8 ofensywnych narzędzi WiFi
├── BLE Arsenal ───────── 5 narzędzi ataku Bluetooth
├── Wywiad ────────────── 3 narzędzia rozpoznania/świadomości
├── Unikanie ──────────── 5 narzędzi ukrycia/anty-wykrywania
├── Combo ─────────────── Predefiniowane + niestandardowe łańcuchy ataków
├── Harmonogram ────────── Opóźnione wykonanie ataków
├── Skrypty ───────────── Przeglądaj 60K+ payloadów z SD
├── Log Sesji ─────────── Rejestrowanie aktywności na SD
├── Jam All ───────────── Jednoczesne zagłuszanie wielu pasm (niestandardowy GUI)
└── Dashboard ─────────── Mobilny interfejs webowy + menedżer plików + OTA
```

---

## Narzędzia

### WiFi Arsenal

| Narzędzie | Co Robi |
|-----------|---------|
| **Network Scanner** | Skanowanie ARP + skan portów TCP. Pokazuje urządzenia, otwarte porty, nazwy producentów |
| **DHCP Starvation** | Zalewa żądaniami DHCP ze sfałszowanymi MAC aby wyczerpać pulę IP routera |
| **Karma Attack** | Odpowiada na wszystkie żądania WiFi probe — pobliskie urządzenia automatycznie łączą się z tobą |
| **DNS Spoofer** | Przechwytuje DNS gdy działa jako AP. Wszystkie domeny rozwiązują do twojego portalu |
| **Portal Templates** | Ładuje niestandardowy phishingowy HTML z karty SD. Przechwytuje dane logowania do pliku |
| **Auto-Phish Portal** | Wykrywa jakie sieci szukają ofiary, klonuje SSID, serwuje portal |
| **Cred Forward** | Przechwytuje dane portalu → automatycznie łączy się z prawdziwym AP → mostuje ruch. Pełny transparentny MITM |
| **WiFi Brute Force** | Generuje inteligentną listę słów z nazwy SSID (kombinacje lat, leet speak, popularne wzorce). Testuje na żywo |

### BLE Arsenal

| Narzędzie | Co Robi |
|-----------|---------|
| **BLE Tracker** | Ciągły skan BLE z oszacowaniem odległości RSSI. Wykrywa AirTagi, SmartTagi, Tile śledzące cię |
| **Name Spammer** | Zalewa kanały advertising BLE setkami losowych nazw urządzeń |
| **AirTag Spoofer** | Nadaje fałszywe sygnały Apple FindMy AirTag. Fantomowe tagi pojawiają się na pobliskich iPhone'ach |
| **Audio Jammer** | Zakłóca połączenia audio BLE (AirPods, głośniki) zalewem advertisementów |
| **Notification Spoofer** | Wysyła fałszywe wyskakujące powiadomienia o znalezionym urządzeniu na Android (Fast Pair), Windows (Swift Pair) i iOS (bliskość AirPods) |

### Wywiad

| Narzędzie | Co Robi |
|-----------|---------|
| **Device Fingerprinter** | Pasywne fingerprintowanie WiFi. Identyfikuje system/model urządzenia na podstawie zachowania probe, OUI, timingu |
| **OPSEC Monitor** | Wykrywa czy jesteś skanowany, deautentykowany lub śledzony. Poziom zagrożenia: zielony/żółty/czerwony |
| **OUI Lookup** | Offline wyszukiwanie producenta MAC z bazy danych na karcie SD |

### Unikanie

| Narzędzie | Co Robi |
|-----------|---------|
| **MAC Rotator** | Randomizuje adres MAC WiFi na konfigurowalnym timerze. Regulowana prędkość |
| **Channel Hopper** | Szybko przełącza kanały WiFi podczas operacji. Konfigurowalny interwał |
| **Decoy Traffic** | Emituje losowe ramki beacon/probe na rotujących kanałach. Maskuje prawdziwą aktywność w szumie |
| **Identity Cloner** | Przechwytuje tożsamości pobliskich urządzeń (MAC + wzorzec probe). Sklonuj jedno by stać się niewidocznym |
| **QR Poisoner** | Generuje kody QR wskazujące na twój portal. Wyświetl na ekranie, wydrukuj i umieść |

### Jam All (Niestandardowy GUI)

Pełnoekranowy panel z przełącznikami per pasmo, paski aktywności na żywo, timer oraz przycisk natychmiastowego zatrzymania.

| Pasmo | Metoda |
|-------|--------|
| WiFi 2.4GHz | Flood deauth cyklujący wszystkie 14 kanałów |
| BLE | Flood kanałów advertisement (37, 38, 39) |
| Sub-GHz | Ciągła transmisja CC1101 (315/433/868/915 MHz) |
| NRF24 | Nośna 2.4GHz na rotujących kanałach |

### Narzędzia Pomocnicze

| Narzędzie | Co Robi |
|-----------|---------|
| **Attack Scheduler** | Wybierz narzędzie → ustaw opóźnienie (0 do 1 godziny) → ustaw czas trwania. Automatycznie startuje i zatrzymuje |
| **Session Log** | Loguje każdą akcję ze znacznikami czasu. Podgląd/eksport/czyszczenie z menu |
| **Script Browser** | Nawiguj po skategoryzowanych folderach SD. Auto-uruchamia na podstawie typu pliku (.txt/.sub/.ir/.html/.nfc/.js) |
| **Combo Presets** | Łańcuchy ataków jednym dotknięciem. Wbudowane: Stealth Mode, Full Attack, Passive Recon. Twórz niestandardowe .txt na SD |
| **Remote Dashboard** | Telefon łączy się z AP ArsenalNet → pełny mobilny interfejs webowy z menedżerem plików + aktualizacje OTA |

---

## Usługi w Tle

Działają persistentnie we wszystkich menu — nie tylko wewnątrz Arsenal.

| Usługa | Opis |
|--------|------|
| **OPSEC Dot** | Kolorowe kółko na pasku stanu (zielony/żółty/czerwony). Zawsze widoczne, na każdym ekranie |
| **Always-On Evasion** | Rotacja MAC + channel hopping + decoy traffic. Działa cicho na rdzeniu 0 CPU |
| **Low Power Mode** | Spowalnia zadania w tle 5x aby oszczędzać baterię. Przełącz z menu Combos |
| **Auto-Dim** | Jasność ekranu spada podczas aktywnych ataków. Przywraca po zatrzymaniu |

---

## Remote Dashboard

Uruchom Arsenal → Dashboard. Podłącz telefon do `ArsenalNet` (hasło: `arsenal32`). Otwórz `192.168.4.1` w przeglądarce.

**Funkcje:**
- Uruchamiaj/zatrzymuj wszystkie 26 narzędzi z telefonu
- Przeglądaj, wgrywaj, usuwaj pliki na karcie SD (folder `/arsenal/`)
- Wgrywaj firmware `.bin` do aktualizacji OTA
- Przeciągnij i upuść skrypty do skategoryzowanych folderów
- Status systemu na żywo (heap, przestrzeń SD, czas pracy)

---

## Układ Karty SD

```
/arsenal/
├── badusb/       Payloady DuckyScript (.txt)
├── subghz/       Przechwycone sygnały Sub-GHz (.sub)
├── ir/           Pliki pilotów IR (.ir)
├── portals/      Szablony HTML evil portal
├── nfc/          Zrzuty kart NFC (.nfc)
├── rfid/         Przechwycenia RFID 125kHz (.rfid)
├── ibutton/      Pliki kluczy iButton (.ibutton)
├── scripts/      Automatyzacja JavaScript (.js)
├── combos/       Niestandardowe presety combo (.txt — jedna nazwa funkcji na linię)
├── logs/         Logi sesji (tworzone automatycznie)
└── creds.txt     Przechwycone dane logowania (tworzone automatycznie)
```

Kompatybilne z [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts) (60K+ skryptów).

---

## Niestandardowe Combo

Utwórz plik `.txt` w `/arsenal/combos/` z jedną funkcją na linię:

```
mac_rotator
channel_hopper
decoy_traffic
ble_tracker
```

Pojawia się automatycznie w Arsenal → Combos.

---

## Sprzęt

Główny cel: **LilyGo T-Embed CC1101**
- ESP32-S3 (16MB flash, 8MB PSRAM)
- Wyświetlacz TFT ST7789 320x170
- Enkoder obrotowy + przycisk
- Radio Sub-GHz CC1101
- Radio NRF24 2.4GHz
- NFC PN532
- Slot karty SD
- USB-C (BadUSB HID)
- Głośnik + Mikrofon
- Diody LED RGB
- Miernik baterii BQ27220

---

## Podziękowania

- Oparty na [Bruce Firmware](https://github.com/pr3y/Bruce) autorstwa pr3y
- Moduł Arsenal autorstwa quietdom
- Kolekcja skryptów kompatybilna z [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts)

---

## Zastrzeżenie

Ten firmware jest przeznaczony wyłącznie do autoryzowanych testów bezpieczeństwa i celów edukacyjnych. Nie używaj przeciwko sieciom lub urządzeniom, których nie posiadasz lub na testowanie których nie masz wyraźnej zgody. Autorzy nie ponoszą odpowiedzialności za niewłaściwe użycie.
