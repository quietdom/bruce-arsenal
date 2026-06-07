🌐 [English](../README.md) | [Português](README_PT.md) | [Italiano](README_IT.md) | [Français](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Türkçe](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Español](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>Firmware ofensivo personalizado para dispositivos ESP32</b><br>
  <i>26 herramientas de ataque • Panel móvil • Evasión en segundo plano • 60K+ scripts</i>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32--S3-blue?style=flat-square" alt="ESP32-S3">
  <img src="https://img.shields.io/badge/Target-T--Embed%20CC1101-green?style=flat-square" alt="T-Embed">
  <img src="https://img.shields.io/badge/Tools-26+-red?style=flat-square" alt="Tools">
  <img src="https://img.shields.io/badge/Scripts-60K+-purple?style=flat-square" alt="Scripts">
  <img src="https://img.shields.io/badge/Status-Active-brightgreen?style=flat-square" alt="Status">
</p>

---

## Qué Es Esto

Una versión profundamente modificada de [Bruce Firmware](https://github.com/pr3y/Bruce) con un módulo **Arsenal** personalizado integrado — un kit completo de seguridad ofensiva accesible desde una única pestaña de menú con icono de mira.

Todas las funcionalidades originales de Bruce permanecen intactas. Arsenal añade 26 nuevas herramientas, un panel web móvil, servicios de evasión en segundo plano y un navegador de scripts que puede cargar más de 60K payloads desde la tarjeta SD.

---

## Instalación

1. Descarga el `.bin` desde [Releases](../../releases)
2. Flashea tu dispositivo usando uno de los métodos:
   - **USB:** [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
   - **OTA:** Después del primer flash, usa Arsenal → Dashboard → sube nuevo .bin desde el teléfono
3. Listo

---

## Menú Arsenal

```
🎯 Arsenal
├── WiFi Arsenal ──────── 8 herramientas ofensivas WiFi
├── BLE Arsenal ───────── 5 herramientas de ataque Bluetooth
├── Inteligencia ──────── 3 herramientas de reconocimiento/alerta
├── Evasión ───────────── 5 herramientas sigilo/anti-detección
├── Combos ────────────── Cadenas de ataque predefinidas + personalizadas
├── Programador ────────── Ejecución de ataques con retardo temporal
├── Scripts ───────────── Navegar 60K+ payloads desde SD
├── Log de Sesión ─────── Registro de actividad en SD
├── Jam All ───────────── Interferencia simultánea multi-banda (GUI personalizada)
└── Dashboard ─────────── UI web móvil + gestor de archivos + OTA
```

---

## Herramientas

### WiFi Arsenal

| Herramienta | Qué Hace |
|-------------|----------|
| **Network Scanner** | Barrido ARP + escaneo de puertos TCP. Muestra dispositivos, puertos abiertos, nombres de fabricantes |
| **DHCP Starvation** | Inunda solicitudes DHCP con MAC falsificados para agotar el pool de IP del router |
| **Karma Attack** | Responde a todas las solicitudes WiFi probe — los dispositivos cercanos se conectan automáticamente a ti |
| **DNS Spoofer** | Intercepta DNS al funcionar como AP. Todos los dominios resuelven a tu portal |
| **Portal Templates** | Carga HTML de phishing personalizado desde la tarjeta SD. Captura credenciales en archivo |
| **Auto-Phish Portal** | Detecta qué redes buscan las víctimas, clona el SSID, sirve el portal |
| **Cred Forward** | Captura credenciales del portal → se conecta automáticamente al AP real → puentea tráfico. MITM transparente completo |
| **WiFi Brute Force** | Genera wordlist inteligente del nombre SSID (combinaciones de año, leet speak, patrones comunes). Prueba en vivo |

### BLE Arsenal

| Herramienta | Qué Hace |
|-------------|----------|
| **BLE Tracker** | Escaneo BLE continuo con estimación de distancia RSSI. Detecta AirTags, SmartTags, Tiles que te siguen |
| **Name Spammer** | Inunda canales de advertising BLE con cientos de nombres de dispositivos aleatorios |
| **AirTag Spoofer** | Emite beacons falsos de Apple FindMy AirTag. Etiquetas fantasma aparecen en iPhones cercanos |
| **Audio Jammer** | Interrumpe conexiones de audio BLE (AirPods, altavoces) con inundación de advertisements |
| **Notification Spoofer** | Envía popups falsos de dispositivo encontrado a Android (Fast Pair), Windows (Swift Pair) e iOS (proximidad AirPods) |

### Inteligencia

| Herramienta | Qué Hace |
|-------------|----------|
| **Device Fingerprinter** | Fingerprinting WiFi pasivo. Identifica SO/modelo del dispositivo por comportamiento probe, OUI, timing |
| **OPSEC Monitor** | Detecta si estás siendo escaneado, desautenticado o rastreado. Nivel de amenaza: verde/amarillo/rojo |
| **OUI Lookup** | Búsqueda offline en base de datos de fabricantes MAC desde tarjeta SD |

### Evasión

| Herramienta | Qué Hace |
|-------------|----------|
| **MAC Rotator** | Aleatoriza la dirección MAC WiFi en temporizador configurable. Velocidad ajustable |
| **Channel Hopper** | Cambia rápidamente canales WiFi durante operaciones. Intervalo configurable |
| **Decoy Traffic** | Emite frames beacon/probe aleatorios en canales rotativos. Enmascara actividad real en ruido |
| **Identity Cloner** | Captura identidades de dispositivos cercanos (MAC + patrón probe). Clona uno para volverte invisible |
| **QR Poisoner** | Genera códigos QR apuntando a tu portal. Muestra en pantalla, imprime y coloca |

### Jam All (GUI Personalizada)

Panel a pantalla completa con toggles por banda, barras de actividad en vivo, cronómetro transcurrido y botón de parada instantánea.

| Banda | Método |
|-------|--------|
| WiFi 2.4GHz | Flood de deauth ciclando los 14 canales |
| BLE | Flood de canales advertisement (37, 38, 39) |
| Sub-GHz | TX continuo CC1101 (315/433/868/915 MHz) |
| NRF24 | Portadora 2.4GHz en canales rotativos |

### Utilidades

| Herramienta | Qué Hace |
|-------------|----------|
| **Attack Scheduler** | Elige herramienta → establece retardo (0 a 1 hora) → establece duración. Se inicia y detiene automáticamente |
| **Session Log** | Registra cada acción con marcas de tiempo. Ver/exportar/borrar desde menú |
| **Script Browser** | Navega carpetas categorizadas de SD. Auto-lanza según tipo de archivo (.txt/.sub/.ir/.html/.nfc/.js) |
| **Combo Presets** | Cadenas de ataque con un toque. Incluidos: Stealth Mode, Full Attack, Passive Recon. Crea personalizados en .txt en SD |
| **Remote Dashboard** | El teléfono se conecta al AP ArsenalNet → UI web móvil completa con gestor de archivos + actualizaciones OTA |

---

## Servicios en Segundo Plano

Estos funcionan persistentemente en todos los menús — no solo dentro de Arsenal.

| Servicio | Descripción |
|----------|-------------|
| **OPSEC Dot** | Círculo de color en la barra de estado (verde/amarillo/rojo). Siempre visible, en cada pantalla |
| **Always-On Evasion** | Rotación MAC + channel hopping + decoy traffic. Se ejecuta silenciosamente en el core 0 de la CPU |
| **Low Power Mode** | Ralentiza tareas en segundo plano 5x para ahorrar batería. Activar desde menú Combos |
| **Auto-Dim** | El brillo de pantalla disminuye durante ataques activos. Se restaura al detenerse |

---

## Remote Dashboard

Inicia Arsenal → Dashboard. Conecta tu teléfono a `ArsenalNet` (contraseña: `arsenal32`). Abre `192.168.4.1` en el navegador.

**Funcionalidades:**
- Inicia/detén las 26 herramientas desde el teléfono
- Navega, sube, elimina archivos en la tarjeta SD (carpeta `/arsenal/`)
- Sube firmware `.bin` para actualización OTA
- Subida de scripts arrastrando y soltando en carpetas categorizadas
- Estado del sistema en vivo (heap, espacio SD, tiempo activo)

---

## Diseño de Tarjeta SD

```
/arsenal/
├── badusb/       Payloads DuckyScript (.txt)
├── subghz/       Capturas de señales Sub-GHz (.sub)
├── ir/           Archivos de control remoto IR (.ir)
├── portals/      Plantillas HTML evil portal
├── nfc/          Volcados de tarjetas NFC (.nfc)
├── rfid/         Capturas RFID 125kHz (.rfid)
├── ibutton/      Archivos de llave iButton (.ibutton)
├── scripts/      Automatización JavaScript (.js)
├── combos/       Presets de combo personalizados (.txt — un nombre de función por línea)
├── logs/         Logs de sesión (creado automáticamente)
└── creds.txt     Credenciales capturadas (creado automáticamente)
```

Compatible con [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts) (60K+ scripts).

---

## Combos Personalizados

Crea un archivo `.txt` en `/arsenal/combos/` con una función por línea:

```
mac_rotator
channel_hopper
decoy_traffic
ble_tracker
```

Aparece automáticamente en Arsenal → Combos.

---

## Hardware

Objetivo principal: **LilyGo T-Embed CC1101**
- ESP32-S3 (16MB flash, 8MB PSRAM)
- Pantalla TFT ST7789 320x170
- Encoder rotativo + botón
- Radio Sub-GHz CC1101
- Radio NRF24 2.4GHz
- NFC PN532
- Ranura para tarjeta SD
- USB-C (BadUSB HID)
- Altavoz + Micrófono
- LEDs RGB
- Medidor de batería BQ27220

---

## Créditos

- Basado en [Bruce Firmware](https://github.com/pr3y/Bruce) por pr3y
- Módulo Arsenal por quietdom
- Colección de scripts compatible con [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts)

---

## Aviso Legal

Este firmware es exclusivamente para pruebas de seguridad autorizadas y fines educativos. No usar contra redes o dispositivos que no poseas o para los que no tengas permiso explícito para probar. Los autores no son responsables del uso indebido.
