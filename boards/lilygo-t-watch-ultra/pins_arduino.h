#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

#define USB_VID 0x303a
#define USB_PID 0x8227

// QSPI AMOLED display
#define DISP_D0  (38)
#define DISP_D1  (39)
#define DISP_D2  (42)
#define DISP_D3  (45)
#define DISP_SCK (40)
#define DISP_CS  (41)
#define DISP_RST (37)
#define DISP_TE  (6)

// Touch
#define TP_INT (12)

// Interrupts
#define RTC_INT    (1)
#define PMU_INT    (7)
#define NFC_INT    (5)
#define SENSOR_INT (8)
#define NFC_CS     (4)

// PDM Microphone
#define MIC_SCK (17)
#define MIC_DAT (18)

// MAX98357A I2S audio
#define I2S_BCLK (9)
#define I2S_WCLK (10)
#define I2S_DOUT (11)

// SD Card
#define SD_CS (21)

// GPS
static const uint8_t TX = 43;
static const uint8_t RX = 44;

// I2C (BHI260, PCF85063A, AXP2101, DRV2605L, CST9217, XL9555 share bus)
static const uint8_t SDA = 3;
static const uint8_t SCL = 2;

// SPI (SD, LoRa, NFC share bus)
static const uint8_t SS = SD_CS;
static const uint8_t MOSI = 34;
static const uint8_t MISO = 33;
static const uint8_t SCK = 35;

// LoRa SX1262
#define LORA_SCK  (SCK)
#define LORA_MISO (MISO)
#define LORA_MOSI (MOSI)
#define LORA_CS   (36)
#define LORA_RST  (47)
#define LORA_BUSY (48)
#define LORA_IRQ  (14)

// GNSS
#define GPS_TX  (TX)
#define GPS_RX  (RX)
#define GPS_PPS (13)

// XL9555 IO Expander
#define EXPANDS_DRV_EN    (6)
#define EXPANDS_DISP_EN   (7)
#define EXPANDS_TOUCH_RST (8)
#define EXPANDS_SD_DET    (10)

// Features
#define USING_XL9555_EXPANDS
#define USING_PCM_AMPLIFIER
#define USING_PDM_MICROPHONE
#define USING_PMU_MANAGE
#define USING_INPUT_DEV_TOUCHPAD
#define USING_ST25R3916
#define USING_BHI260_SENSOR
#define HAS_SD_CARD_SOCKET

#endif /* Pins_Arduino_h */
