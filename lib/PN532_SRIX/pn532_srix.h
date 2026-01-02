/*
 * This file is part of arduino-pn532-srix.
 * arduino-pn532-srix is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * arduino-pn532-srix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with arduino-pn532-srix.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author Lilz
 * @license  GNU Lesser General Public License v3.0 (see license.txt)
 * @Some small mod from Senape3000
 *  This is a library for the communication with an I2C PN532 NFC/RFID breakout board.
 *  adapted from Adafruit's library.
 *  This library supports only I2C to communicate.
 */

#ifndef PN532_SRIX_H
#define PN532_SRIX_H

#include "Arduino.h" // Arduino header

// Settings
// Uncomment/comment to enable/disable debug
// #define PN532DEBUG
#define PN532DEBUGPRINT Serial // If debug is enabled, specify the serial

// PN532 Commands
#define PN532_COMMAND_GETFIRMWAREVERSION (0x02)
#define PN532_COMMAND_SAMCONFIGURATION (0x14)
#define PN532_COMMAND_RFCONFIGURATION (0x32)
#define PN532_COMMAND_INLISTPASSIVETARGET (0x4A)
#define PN532_COMMAND_INCOMMUNICATETHRU (0x42)

// PN532 I2C
#define PN532_I2C_ADDRESS (0x48 >> 1)
#define PN532_PREAMBLE (0x00)
#define PN532_POSTAMBLE (0x00)
#define PN532_STARTCODE (0xFF)
#define PN532_HOSTTOPN532 (0xD4)
#define PN532_PN532TOHOST (0xD5)

// SRIX4K
#define SRIX4K_INITIATE (0x06)
#define SRIX4K_SELECT (0x0E)
#define SRIX4K_READBLOCK (0x08)
#define SRIX4K_WRITEBLOCK (0x09)
#define SRIX4K_GETUID (0x0B)

class Arduino_PN532_SRIX {
public:
    Arduino_PN532_SRIX(uint8_t irq, uint8_t reset); // Hardware I2C
    Arduino_PN532_SRIX();
    bool init(void);

    // Generic PN532 functions
    uint32_t getFirmwareVersion(void);
    bool setPassiveActivationRetries(uint8_t maxRetries);

    // SRIX functions
    bool SRIX_init(void);
    bool SRIX_initiate_select(void);
    bool SRIX_read_block(uint8_t address, uint8_t *block);
    bool SRIX_write_block(uint8_t address, uint8_t *block);
    bool SRIX_get_uid(uint8_t *block);

private:
    uint8_t _irq = 0, _reset = 0;

    // PN532 Init (called by init function)
    bool SAMConfig(void);

    // High level communication functions that handle I2C.
    void readData(uint8_t *buffer, uint8_t n);
    bool readACK();
    bool isReady();
    bool waitReady(uint16_t timeout);

    // Send command to PN532 over I2C
    void writeCommand(uint8_t *command, uint8_t commandLength);
    bool sendCommandCheckAck(uint8_t *command, uint8_t commandLength, uint16_t timeout = 100);
};

#endif // PN532_SRIX_H
