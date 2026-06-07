🌐 [English](../README.md) | [Português](README_PT.md) | [Italiano](README_IT.md) | [Français](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Türkçe](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Español](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>Firmware keamanan ofensif kustom untuk perangkat ESP32</b><br>
  <i>26 alat serangan • Dasbor mobile • Penghindaran latar belakang • 60K+ skrip</i>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32--S3-blue?style=flat-square" alt="ESP32-S3">
  <img src="https://img.shields.io/badge/Target-T--Embed%20CC1101-green?style=flat-square" alt="T-Embed">
  <img src="https://img.shields.io/badge/Tools-26+-red?style=flat-square" alt="Tools">
  <img src="https://img.shields.io/badge/Scripts-60K+-purple?style=flat-square" alt="Scripts">
  <img src="https://img.shields.io/badge/Status-Active-brightgreen?style=flat-square" alt="Status">
</p>

---

## Apa Ini

Versi yang dimodifikasi secara mendalam dari [Bruce Firmware](https://github.com/pr3y/Bruce) dengan modul **Arsenal** kustom yang terintegrasi — toolkit keamanan ofensif lengkap yang dapat diakses dari satu tab menu dengan ikon bidik.

Semua fitur asli Bruce tetap utuh. Arsenal menambahkan 26 alat baru, dasbor web mobile, layanan penghindaran latar belakang, dan browser skrip yang dapat memuat 60K+ payload dari kartu SD.

---

## Instalasi

1. Unduh file `.bin` dari [Releases](../../releases)
2. Flash ke perangkat Anda menggunakan salah satu metode:
   - **USB:** [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
   - **OTA:** Setelah flash pertama, gunakan Arsenal → Dashboard → unggah .bin baru dari ponsel
3. Selesai

---

## Menu Arsenal

```
🎯 Arsenal
├── WiFi Arsenal ──────── 8 alat ofensif WiFi
├── BLE Arsenal ───────── 5 alat serangan Bluetooth
├── Intelijen ─────────── 3 alat pengintaian/kesadaran
├── Penghindaran ──────── 5 alat siluman/anti-deteksi
├── Combo ─────────────── Rantai serangan preset + kustom
├── Penjadwal ─────────── Eksekusi serangan tertunda waktu
├── Skrip ─────────────── Jelajahi 60K+ payload dari SD
├── Log Sesi ──────────── Pencatatan aktivitas ke SD
├── Jam All ───────────── Jamming multi-band simultan (GUI kustom)
└── Dashboard ─────────── UI web mobile + manajer file + OTA
```

---

## Alat

### WiFi Arsenal

| Alat | Fungsinya |
|------|-----------|
| **Network Scanner** | Sweep ARP + scan port TCP. Menampilkan perangkat, port terbuka, nama vendor |
| **DHCP Starvation** | Membanjiri permintaan DHCP dengan MAC palsu untuk menguras pool IP router |
| **Karma Attack** | Merespons semua permintaan WiFi probe — perangkat di dekat otomatis terhubung ke Anda |
| **DNS Spoofer** | Menyadap DNS saat berjalan sebagai AP. Semua domain mengarah ke portal Anda |
| **Portal Templates** | Memuat HTML phishing kustom dari kartu SD. Menangkap kredensial ke file |
| **Auto-Phish Portal** | Mendeteksi jaringan apa yang dicari korban, mengkloning SSID, menyajikan portal |
| **Cred Forward** | Menangkap kredensial portal → otomatis terhubung ke AP asli → menjembatani traffic. MITM transparan penuh |
| **WiFi Brute Force** | Menghasilkan wordlist pintar dari nama SSID (kombinasi tahun, leet speak, pola umum). Mencoba langsung |

### BLE Arsenal

| Alat | Fungsinya |
|------|-----------|
| **BLE Tracker** | Scan BLE berkelanjutan dengan estimasi jarak RSSI. Mendeteksi AirTag, SmartTag, Tile yang mengikuti Anda |
| **Name Spammer** | Membanjiri saluran advertising BLE dengan ratusan nama perangkat acak |
| **AirTag Spoofer** | Menyiarkan beacon Apple FindMy AirTag palsu. Tag hantu muncul di iPhone terdekat |
| **Audio Jammer** | Mengganggu koneksi audio BLE (AirPods, speaker) dengan banjir advertisement |
| **Notification Spoofer** | Mendorong popup palsu perangkat ditemukan ke Android (Fast Pair), Windows (Swift Pair), dan iOS (kedekatan AirPods) |

### Intelijen

| Alat | Fungsinya |
|------|-----------|
| **Device Fingerprinter** | Fingerprinting WiFi pasif. Mengidentifikasi OS/model perangkat dari perilaku probe, OUI, timing |
| **OPSEC Monitor** | Mendeteksi jika Anda sedang di-scan, di-deauth, atau dilacak. Level ancaman: hijau/kuning/merah |
| **OUI Lookup** | Pencarian offline database vendor MAC dari kartu SD |

### Penghindaran

| Alat | Fungsinya |
|------|-----------|
| **MAC Rotator** | Mengacak alamat MAC WiFi pada timer yang dapat dikonfigurasi. Kecepatan yang dapat disesuaikan |
| **Channel Hopper** | Berpindah kanal WiFi dengan cepat selama operasi. Interval yang dapat dikonfigurasi |
| **Decoy Traffic** | Memancarkan frame beacon/probe acak pada kanal berputar. Menyamarkan aktivitas nyata dalam noise |
| **Identity Cloner** | Menangkap identitas perangkat terdekat (MAC + pola probe). Kloning satu untuk menjadi tak terlihat |
| **QR Poisoner** | Menghasilkan kode QR yang mengarah ke portal Anda. Tampilkan di layar, cetak dan tempatkan |

### Jam All (GUI Kustom)

Dasbor layar penuh dengan toggle per band, bar aktivitas langsung, timer berlalu, dan tombol berhenti darurat.

| Band | Metode |
|------|--------|
| WiFi 2.4GHz | Banjir deauth berputar di semua 14 kanal |
| BLE | Banjir saluran advertisement (37, 38, 39) |
| Sub-GHz | TX berkelanjutan CC1101 (315/433/868/915 MHz) |
| NRF24 | Carrier 2.4GHz pada kanal berputar |

### Utilitas

| Alat | Fungsinya |
|------|-----------|
| **Attack Scheduler** | Pilih alat → atur penundaan (0 sampai 1 jam) → atur durasi. Otomatis mulai dan berhenti |
| **Session Log** | Mencatat setiap aksi dengan timestamp. Lihat/ekspor/hapus dari menu |
| **Script Browser** | Jelajahi folder SD terkategorisasi. Otomatis meluncurkan berdasarkan tipe file (.txt/.sub/.ir/.html/.nfc/.js) |
| **Combo Presets** | Rantai serangan satu sentuhan. Bawaan: Stealth Mode, Full Attack, Passive Recon. Buat kustom .txt di SD |
| **Remote Dashboard** | Ponsel terhubung ke AP ArsenalNet → UI web mobile lengkap dengan manajer file + pembaruan OTA |

---

## Layanan Latar Belakang

Ini berjalan secara persisten di semua menu — tidak hanya di dalam Arsenal.

| Layanan | Deskripsi |
|---------|-----------|
| **OPSEC Dot** | Lingkaran berwarna di bilah status (hijau/kuning/merah). Selalu terlihat, di setiap layar |
| **Always-On Evasion** | Rotasi MAC + channel hopping + decoy traffic. Berjalan diam-diam di core CPU 0 |
| **Low Power Mode** | Memperlambat tugas latar belakang 5x untuk menghemat baterai. Toggle dari menu Combos |
| **Auto-Dim** | Kecerahan layar turun selama serangan aktif. Kembali normal saat berhenti |

---

## Remote Dashboard

Mulai Arsenal → Dashboard. Hubungkan ponsel ke `ArsenalNet` (kata sandi: `arsenal32`). Buka `192.168.4.1` di browser.

**Fitur:**
- Mulai/hentikan semua 26 alat dari ponsel
- Jelajahi, unggah, hapus file di kartu SD (folder `/arsenal/`)
- Unggah firmware `.bin` untuk pembaruan OTA
- Seret dan lepas unggah skrip ke folder terkategorisasi
- Status sistem langsung (heap, ruang SD, uptime)

---

## Tata Letak Kartu SD

```
/arsenal/
├── badusb/       Payload DuckyScript (.txt)
├── subghz/       Tangkapan sinyal Sub-GHz (.sub)
├── ir/           File remote IR (.ir)
├── portals/      Template HTML evil portal
├── nfc/          Dump kartu NFC (.nfc)
├── rfid/         Tangkapan RFID 125kHz (.rfid)
├── ibutton/      File kunci iButton (.ibutton)
├── scripts/      Otomasi JavaScript (.js)
├── combos/       Preset combo kustom (.txt — satu nama fitur per baris)
├── logs/         Log sesi (otomatis dibuat)
└── creds.txt     Kredensial yang ditangkap (otomatis dibuat)
```

Kompatibel dengan [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts) (60K+ skrip).

---

## Combo Kustom

Buat file `.txt` di `/arsenal/combos/` dengan satu fitur per baris:

```
mac_rotator
channel_hopper
decoy_traffic
ble_tracker
```

Otomatis muncul di Arsenal → Combos.

---

## Perangkat Keras

Target utama: **LilyGo T-Embed CC1101**
- ESP32-S3 (16MB flash, 8MB PSRAM)
- Layar TFT ST7789 320x170
- Rotary encoder + tombol
- Radio Sub-GHz CC1101
- Radio NRF24 2.4GHz
- NFC PN532
- Slot kartu SD
- USB-C (BadUSB HID)
- Speaker + Mikrofon
- LED RGB
- Pengukur baterai BQ27220

---

## Kredit

- Berdasarkan [Bruce Firmware](https://github.com/pr3y/Bruce) oleh pr3y
- Modul Arsenal oleh quietdom
- Koleksi skrip kompatibel dengan [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts)

---

## Disclaimer

Firmware ini hanya untuk pengujian keamanan yang diotorisasi dan tujuan pendidikan. Jangan gunakan terhadap jaringan atau perangkat yang tidak Anda miliki atau tidak memiliki izin eksplisit untuk menguji. Penulis tidak bertanggung jawab atas penyalahgunaan.
