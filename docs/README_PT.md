🌐 [English](../README.md) | [Português](README_PT.md) | [Italiano](README_IT.md) | [Français](README_FR.md) | [Русский](README_RU.md) | [中文](README_ZH.md) | [Polski](README_PL.md) | [Nederlands](README_NL.md) | [Türkçe](README_TR.md) | [Deutsch](README_DE.md) | [Tiếng Việt](README_VN.md) | [Español](README_ES.md) | [Indonesia](README_ID.md) | [العربية](README_AR.md)

<p align="center">
  <img src="../media/bruce_banner.jpg" alt="Bruce Arsenal" width="700">
</p>

<h1 align="center">🎯 Bruce Arsenal</h1>

<p align="center">
  <b>Firmware ofensivo customizado para dispositivos ESP32</b><br>
  <i>26 ferramentas de ataque • Painel móvel • Evasão em segundo plano • 60K+ scripts</i>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32--S3-blue?style=flat-square" alt="ESP32-S3">
  <img src="https://img.shields.io/badge/Target-T--Embed%20CC1101-green?style=flat-square" alt="T-Embed">
  <img src="https://img.shields.io/badge/Tools-26+-red?style=flat-square" alt="Tools">
  <img src="https://img.shields.io/badge/Scripts-60K+-purple?style=flat-square" alt="Scripts">
  <img src="https://img.shields.io/badge/Status-Active-brightgreen?style=flat-square" alt="Status">
</p>

---

## O Que É Isto

Uma versão altamente modificada do [Bruce Firmware](https://github.com/pr3y/Bruce) com um módulo **Arsenal** customizado integrado — um kit completo de segurança ofensiva acessível a partir de uma única aba de menu com ícone de mira.

Todas as funcionalidades originais do Bruce permanecem intactas. O Arsenal adiciona 26 novas ferramentas, um painel web móvel, serviços de evasão em segundo plano e um navegador de scripts que pode carregar mais de 60K payloads do cartão SD.

---

## Instalação

1. Baixe o `.bin` em [Releases](../../releases)
2. Grave no seu dispositivo usando um dos métodos:
   - **USB:** [ESP Web Flasher](https://espressif.github.io/esptool-js/) (Chrome/Edge)
   - **OTA:** Após a primeira gravação, use Arsenal → Dashboard → envie novo .bin pelo celular
3. Pronto

---

## Menu Arsenal

```
🎯 Arsenal
├── WiFi Arsenal ──────── 8 ferramentas ofensivas WiFi
├── BLE Arsenal ───────── 5 ferramentas de ataque Bluetooth
├── Inteligência ──────── 3 ferramentas de reconhecimento/alerta
├── Evasão ────────────── 5 ferramentas de furtividade/anti-detecção
├── Combos ────────────── Cadeias de ataque predefinidas + customizadas
├── Agendador ─────────── Execução de ataques com atraso temporal
├── Scripts ───────────── Navegue 60K+ payloads do SD
├── Log de Sessão ─────── Registro de atividades no SD
├── Jam All ───────────── Jamming simultâneo multi-banda (GUI customizada)
└── Dashboard ─────────── UI web móvel + gerenciador de arquivos + OTA
```

---

## Ferramentas

### WiFi Arsenal

| Ferramenta | O Que Faz |
|------------|-----------|
| **Network Scanner** | Varredura ARP + scan de portas TCP. Mostra dispositivos, portas abertas, nomes de fabricantes |
| **DHCP Starvation** | Inunda requisições DHCP com MACs falsificados para esgotar o pool de IPs do roteador |
| **Karma Attack** | Responde a todas as requisições WiFi probe — dispositivos próximos se conectam automaticamente a você |
| **DNS Spoofer** | Intercepta DNS quando operando como AP. Todos os domínios resolvem para seu portal |
| **Portal Templates** | Carrega HTML de phishing customizado do cartão SD. Captura credenciais em arquivo |
| **Auto-Phish Portal** | Detecta quais redes as vítimas estão procurando, clona o SSID, serve o portal |
| **Cred Forward** | Captura credenciais do portal → conecta-se automaticamente ao AP real → faz ponte do tráfego. MITM transparente completo |
| **WiFi Brute Force** | Gera wordlist inteligente a partir do nome do SSID (combinações de ano, leet speak, padrões comuns). Testa em tempo real |

### BLE Arsenal

| Ferramenta | O Que Faz |
|------------|-----------|
| **BLE Tracker** | Scan BLE contínuo com estimativa de distância por RSSI. Detecta AirTags, SmartTags, Tiles te seguindo |
| **Name Spammer** | Inunda canais de advertising BLE com centenas de nomes de dispositivos aleatórios |
| **AirTag Spoofer** | Transmite beacons falsos de Apple FindMy AirTag. Tags fantasmas aparecem em iPhones próximos |
| **Audio Jammer** | Interrompe conexões de áudio BLE (AirPods, caixas de som) com inundação de advertisements |
| **Notification Spoofer** | Envia popups falsos de dispositivo encontrado para Android (Fast Pair), Windows (Swift Pair) e iOS (proximidade AirPods) |

### Inteligência

| Ferramenta | O Que Faz |
|------------|-----------|
| **Device Fingerprinter** | Fingerprinting WiFi passivo. Identifica SO/modelo do dispositivo pelo comportamento de probe, OUI, timing |
| **OPSEC Monitor** | Detecta se você está sendo escaneado, desautenticado ou rastreado. Nível de ameaça: verde/amarelo/vermelho |
| **OUI Lookup** | Consulta offline de banco de dados de fabricantes MAC no cartão SD |

### Evasão

| Ferramenta | O Que Faz |
|------------|-----------|
| **MAC Rotator** | Randomiza o endereço MAC WiFi em temporizador configurável. Velocidade ajustável |
| **Channel Hopper** | Alterna rapidamente canais WiFi durante operações. Intervalo configurável |
| **Decoy Traffic** | Emite frames de beacon/probe aleatórios em canais rotativos. Mascara atividade real em ruído |
| **Identity Cloner** | Captura identidades de dispositivos próximos (MAC + padrão de probe). Clone um para se tornar invisível |
| **QR Poisoner** | Gera QR codes apontando para seu portal. Exibe na tela, imprima e coloque |

### Jam All (GUI Customizada)

Painel em tela cheia com toggles por banda, barras de atividade ao vivo, cronômetro decorrido e botão de parada instantânea.

| Banda | Método |
|-------|--------|
| WiFi 2.4GHz | Flood de deauth ciclando todos os 14 canais |
| BLE | Flood em canais de advertisement (37, 38, 39) |
| Sub-GHz | TX contínuo CC1101 (315/433/868/915 MHz) |
| NRF24 | Portadora 2.4GHz em canais rotativos |

### Utilidades

| Ferramenta | O Que Faz |
|------------|-----------|
| **Attack Scheduler** | Escolha uma ferramenta → defina atraso (0 a 1 hora) → defina duração. Inicia e para automaticamente |
| **Session Log** | Registra toda ação com timestamps. Visualize/exporte/limpe pelo menu |
| **Script Browser** | Navegue pastas categorizadas do SD. Auto-executa baseado no tipo de arquivo (.txt/.sub/.ir/.html/.nfc/.js) |
| **Combo Presets** | Cadeias de ataque com um toque. Inclusos: Stealth Mode, Full Attack, Passive Recon. Crie customizados em .txt no SD |
| **Remote Dashboard** | Celular conecta ao AP ArsenalNet → UI web móvel completa com gerenciador de arquivos + atualizações OTA |

---

## Serviços em Segundo Plano

Estes rodam persistentemente em todos os menus — não apenas dentro do Arsenal.

| Serviço | Descrição |
|---------|-----------|
| **OPSEC Dot** | Círculo colorido na barra de status (verde/amarelo/vermelho). Sempre visível, em todas as telas |
| **Always-On Evasion** | Rotação de MAC + channel hopping + decoy traffic. Roda silenciosamente no core 0 da CPU |
| **Low Power Mode** | Reduz tarefas em segundo plano 5x para economizar bateria. Ative pelo menu Combos |
| **Auto-Dim** | Brilho da tela reduz durante ataques ativos. Restaura ao parar |

---

## Remote Dashboard

Inicie Arsenal → Dashboard. Conecte seu celular ao `ArsenalNet` (senha: `arsenal32`). Abra `192.168.4.1` no navegador.

**Funcionalidades:**
- Inicie/pare todas as 26 ferramentas pelo celular
- Navegue, envie, delete arquivos no cartão SD (pasta `/arsenal/`)
- Envie firmware `.bin` para atualização OTA
- Upload de scripts com arrastar e soltar para pastas categorizadas
- Status do sistema ao vivo (heap, espaço SD, tempo ligado)

---

## Layout do Cartão SD

```
/arsenal/
├── badusb/       Payloads DuckyScript (.txt)
├── subghz/       Capturas de sinais Sub-GHz (.sub)
├── ir/           Arquivos de controle remoto IR (.ir)
├── portals/      Templates HTML de evil portal
├── nfc/          Dumps de cartões NFC (.nfc)
├── rfid/         Capturas RFID 125kHz (.rfid)
├── ibutton/      Arquivos de chave iButton (.ibutton)
├── scripts/      Automação JavaScript (.js)
├── combos/       Presets de combo customizados (.txt — um nome de feature por linha)
├── logs/         Logs de sessão (criado automaticamente)
└── creds.txt     Credenciais capturadas (criado automaticamente)
```

Compatível com [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts) (60K+ scripts).

---

## Combos Customizados

Crie um arquivo `.txt` em `/arsenal/combos/` com uma feature por linha:

```
mac_rotator
channel_hopper
decoy_traffic
ble_tracker
```

Aparece automaticamente em Arsenal → Combos.

---

## Hardware

Alvo principal: **LilyGo T-Embed CC1101**
- ESP32-S3 (16MB flash, 8MB PSRAM)
- Display TFT ST7789 320x170
- Encoder rotativo + botão
- Rádio Sub-GHz CC1101
- Rádio NRF24 2.4GHz
- NFC PN532
- Slot para cartão SD
- USB-C (BadUSB HID)
- Alto-falante + Microfone
- LEDs RGB
- Medidor de bateria BQ27220

---

## Créditos

- Baseado no [Bruce Firmware](https://github.com/pr3y/Bruce) por pr3y
- Módulo Arsenal por quietdom
- Coleção de scripts compatível com [BruceFlipperScripts](https://github.com/Unknown3613/BruceFlipperScripts)

---

## Aviso Legal

Este firmware é destinado exclusivamente para testes de segurança autorizados e fins educacionais. Não utilize contra redes ou dispositivos que você não possui ou não tem permissão explícita para testar. Os autores não são responsáveis pelo uso indevido.
