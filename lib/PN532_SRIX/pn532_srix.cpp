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

#include "pn532_srix.h" // Current library header

#include <Wire.h>

#ifdef __SAM3X8E__ // Arduino Due specific code
#define WIRE Wire1
#else
#define WIRE Wire
#endif

// Fix per ESP32 I2C clock stretching
#ifdef ESP32
#define I2C_TIMEOUT 100
#endif

// Static variables to avoid conflicts with Adafruit_PN532
static byte pn532ack[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};
static byte pn532response_firmwarevers[] = {0x00, 0x00, 0xFF, 0x06, 0xFA, 0xD5};
#define PN532_PACKBUFFSIZ 64
static byte pn532_packetbuffer[PN532_PACKBUFFSIZ];

/************** high level communication functions (handle I2C) **************/

// Fix per ESP32 I2C clock stretching
#ifdef ESP32
#define I2C_TIMEOUT 100
#endif

/**
 * i2c_send sends a single byte via I2C.
 * @param toSend the byte to send.
 */
static inline void i2c_send(uint8_t toSend) {
// Wire.h api has been changed on Arduino 1.0
#if ARDUINO >= 100
    WIRE.write((uint8_t)toSend);
#else
    WIRE.send(x);
#endif
}

/**
 * i2c_receive reads a single byte via I2C.
 * @return read byte value.
 */
static inline uint8_t i2c_receive(void) {
#if ARDUINO >= 100
    return WIRE.read();
#else
    return WIRE.receive();
#endif
}

/**
 * Reads n bytes of data from the PN532 via I2C.
 * @param  buffer pointer to the buffer where data will be written.
 * @param  n number of bytes to be read.
 */
void Arduino_PN532_SRIX::readData(uint8_t *buffer, uint8_t n) {
    // I2C
    uint16_t timer = 0;
    delay(2);

#ifdef PN532DEBUG
    PN532DEBUGPRINT.print(F("Reading: "));
#endif

    // Start read (n+1 to take into account leading 0x01 with I2C)
    WIRE.requestFrom((uint8_t)PN532_I2C_ADDRESS, (uint8_t)(n + 2));
    // Discard the leading 0x01
    i2c_receive();
    for (uint8_t i = 0; i < n; i++) {
        delay(1);
        buffer[i] = i2c_receive();
#ifdef PN532DEBUG
        // Print current byte as HEX
        PN532DEBUGPRINT.print(F(" 0x"));
        PN532DEBUGPRINT.print(buffer[i], HEX);
#endif
    }

#ifdef PN532DEBUG
    // Empty line
    PN532DEBUGPRINT.println();
#endif
}

/**
 * Check if PN532 returned ACK is valid.
 * @return success. If it's false there is a problem.
 */
bool Arduino_PN532_SRIX::readACK() {
    // Array to save ACK
    uint8_t ackBuffer[6];

    // Read I2C data
    readData(ackBuffer, 6);

    // Compare ACK with an expected result
    return memcmp((char *)ackBuffer, (char *)pn532ack, 6) == 0;
}

/**
 * Check if there is a response ready.
 * @return true if the PN532 is ready with a response.
 */
bool Arduino_PN532_SRIX::isReady() {
    // Solo se IRQ è definito
    if (_irq == 255) {
        return true; // Nessun IRQ, usa sempre polling
    }
    return digitalRead(_irq) == 0;
}

/**
 * Waits until the PN532 is ready.
 * @param timeout timeout before giving up. If timeout is 0, cycle will be infinite.
 * @return true if a target has been found.
 */
bool Arduino_PN532_SRIX::waitReady(uint16_t timeout) {
    // milliseconds timer to check timeout
    uint16_t timer = 0;

    // Iterate while PN532 isn't ready or there is a timeout
    while (!isReady()) {
        // If timeout == 0, ignore it
        if (timeout != 0) {
            // Add 10ms to timer (delay time)
            timer += 10;

            // Check if there is a timeout
            if (timer > timeout) {
#ifdef PN532DEBUG
                PN532DEBUGPRINT.println("TIMEOUT!");
#endif
                return false;
            }
        }

        // Delay for next cycle
        delay(10);
    }

    // PN532 is ready!
    return true;
}

/**
 * Writes a command to the PN532, automatically inserting the preamble and required frame details (checksum,
 * len, etc.).
 * @param command pointer to the command buffer.
 * @param commandLength command size in bytes.
 */
void Arduino_PN532_SRIX::writeCommand(uint8_t *command, uint8_t commandLength) {
    // I2C
    uint8_t checksum;
    commandLength++;

#ifdef PN532DEBUG
    PN532DEBUGPRINT.print(F("\nSending: "));
#endif

    delay(2); // or whatever the delay is for waking up the board

    // I2C START
    WIRE.beginTransmission(PN532_I2C_ADDRESS);
    checksum = PN532_PREAMBLE + PN532_PREAMBLE + PN532_STARTCODE;
    i2c_send(PN532_PREAMBLE);
    i2c_send(PN532_PREAMBLE);
    i2c_send(PN532_STARTCODE);

    // I2C final length
    i2c_send(commandLength);
    i2c_send(~commandLength + 1);

    // Direction (host to pn)
    i2c_send(PN532_HOSTTOPN532);
    checksum += PN532_HOSTTOPN532;

    // Send command
    for (uint8_t i = 0; i < commandLength - 1; i++) {
        i2c_send(command[i]);
        checksum += command[i];
#ifdef PN532DEBUG
        PN532DEBUGPRINT.print(F(" 0x"));
        PN532DEBUGPRINT.print((byte)command[i], HEX);
#endif
    }

    // Send checksum and close I2C transmission
    i2c_send((byte)~checksum);
    i2c_send((byte)PN532_POSTAMBLE);

    // I2C STOP
    WIRE.endTransmission();
}

/**
 * Sends a command and waits a specified period (timeout) for the ACK.
 * @param  command pointer to the command buffer.
 * @param  commandLenght the size of the command in bytes.
 * @param  timeout timeout before giving up (default value is 1 second).
 * @return true if everything is OK, false if timeout occurred before an ACK was received.
 */
bool Arduino_PN532_SRIX::sendCommandCheckAck(uint8_t *command, uint8_t commandLenght, uint16_t timeout) {
    // default timeout of one second
    // write the command
    writeCommand(command, commandLenght);

    // Wait for chip to say its ready!
    if (!waitReady(timeout)) { return false; }

#ifdef PN532DEBUG
    PN532DEBUGPRINT.println(F("\nI2C IRQ received"));
#endif

    // Check acknowledgement
    if (!readACK()) {
#ifdef PN532DEBUG
        PN532DEBUGPRINT.println(F("\nNo ACK frame received!"));
#endif
        return false;
    }

    return true; // ACK is valid
}

/***** Initialization ******/

/**
 * Instantiates a new Arduino_PN532 class using I2C with hardware pins.
 * @param irq    Location of the IRQ pin (-1 to disable)
 * @param reset  Location of the RSTPD_N pin (-1 to disable)
 */
Arduino_PN532_SRIX::Arduino_PN532_SRIX(uint8_t irq, uint8_t reset) {
    _irq = irq;
    _reset = reset;

    // Inizializza i pin solo se sono validi (>= 0)
    if (_irq != 255 && _irq >= 0) { // uint8_t(-1) = 255
        pinMode(_irq, INPUT);
    }
    if (_reset != 255 && _reset >= 0) { pinMode(_reset, OUTPUT); }
}

/**
 * Instantiates a new Arduino_PN532 class using I2C without hardware pins.
 * Used for external I2C PN532 modules (polling mode only).
 */
Arduino_PN532_SRIX::Arduino_PN532_SRIX() {
    _irq = 255;   // uint8_t(-1)
    _reset = 255; // uint8_t(-1)
}

/**
 * Setups the hardware (begin I2C communication) and configure PN532 SAM
 * @return success. False if there is an error.
 */
bool Arduino_PN532_SRIX::init() {
    // I2C initialization.
    WIRE.begin();

    // Reset the PN532 solo se il pin è valido
    if (_reset != 255) {
        digitalWrite(_reset, HIGH);
        digitalWrite(_reset, LOW);
        delay(400);
        digitalWrite(_reset, HIGH);
    }

    // Small delay required before taking other actions after reset.
    delay(10);

    return SAMConfig();
}

/***** PN532 Commands ******/

/**
 * Checks the firmware version of the PN5xx chip
 * @return The chip's firmware version and ID. 0 if there is an error.
 */
uint32_t Arduino_PN532_SRIX::getFirmwareVersion(void) {
    uint32_t response;
    pn532_packetbuffer[0] = PN532_COMMAND_GETFIRMWAREVERSION;

    if (!sendCommandCheckAck(pn532_packetbuffer, 1)) {
#ifdef PN532DEBUG
        PN532DEBUGPRINT.println(F("Unable to send Get Firmware command!"));
#endif
        return 0;
    }

    // read data packet
    readData(pn532_packetbuffer, 12);

    // check if firmware version corresponds
    if (memcmp((char *)pn532_packetbuffer, (char *)pn532response_firmwarevers, 6) != 0) {
#ifdef PN532DEBUG
        PN532DEBUGPRINT.println(F("Firmware version doesn't match!"));
#endif
        return 0;
    }

    // Get version and write to an uint32
    response = pn532_packetbuffer[7];
    response <<= 8;
    response |= pn532_packetbuffer[8];
    response <<= 8;
    response |= pn532_packetbuffer[9];
    response <<= 8;
    response |= pn532_packetbuffer[10];

    return response;
}

/**
 * Sets the MxRtyPassiveActivation byte of the RFConfiguration register.
 * @param  maxRetries  0xFF to wait forever, 0x00..0xFE to timeout after mxRetries.
 * @return true if everything executed properly, false if there is an error.
 */
bool Arduino_PN532_SRIX::setPassiveActivationRetries(uint8_t maxRetries) {
    pn532_packetbuffer[0] = PN532_COMMAND_RFCONFIGURATION;
    pn532_packetbuffer[1] = 5;    // Config item 5 (MaxRetries)
    pn532_packetbuffer[2] = 0xFF; // MxRtyATR (default = 0xFF)
    pn532_packetbuffer[3] = 0x01; // MxRtyPSL (default = 0x01)
    pn532_packetbuffer[4] = maxRetries;

    // Check (true = success)
    return sendCommandCheckAck(pn532_packetbuffer, 5);
}

/**
 * Disable the SAM (Secure Access Module) to set PN532 in normal mode.
 * A SAM companion chip can be used to bring security. It is connected to the PN532 by using a S2C interface.
 * However we don't use it so we will disable the interface using Normal mode.
 * @return success. If it's false there is an error.
 */
bool Arduino_PN532_SRIX::SAMConfig(void) {
    pn532_packetbuffer[0] = PN532_COMMAND_SAMCONFIGURATION;
    pn532_packetbuffer[1] = 0x01; // normal mode
    pn532_packetbuffer[2] = 0x14; // timeout 50ms * 20 = 1 second
    pn532_packetbuffer[3] = 0x01; // use IRQ pin!

    if (!sendCommandCheckAck(pn532_packetbuffer, 4)) { return false; }

    // read data packet
    readData(pn532_packetbuffer, 8);

    // Check returned result
    return (pn532_packetbuffer[6] == 0x15);
}

/***** SRIX4K functions ******/

/**
 * Initialize a SRIX4K (list passive target)
 * @return success. If it's false there is an error.
 */
bool Arduino_PN532_SRIX::SRIX_init(void) {
    // Send PN532 InlistPassiveTarget command
    pn532_packetbuffer[0] = PN532_COMMAND_INLISTPASSIVETARGET;
    pn532_packetbuffer[1] = 0x01; // Max 1 target at once
    pn532_packetbuffer[2] = 0x03; // Set target to 106 kbps type B (ISO/IEC14443-3B)
    pn532_packetbuffer[3] = 0x00; // Init data

    // Check ACK
    if (!sendCommandCheckAck(pn532_packetbuffer, 4)) {
#ifdef PN532DEBUG
        PN532DEBUGPRINT.println(F("List targets command failed"));
#endif
        return false;
    }

    return true;
}

/**
 * Send INIT and SELECT command (SRIX4K)
 * @return success. If it's false there is an error.
 */
bool Arduino_PN532_SRIX::SRIX_initiate_select(void) {
    /*** INITIATE ***/
    uint8_t chip_id;

    // Send SRIX4K command INIT
    pn532_packetbuffer[0] = PN532_COMMAND_INCOMMUNICATETHRU;
    pn532_packetbuffer[1] = SRIX4K_INITIATE;
    pn532_packetbuffer[2] = 0x00;

    if (!sendCommandCheckAck(pn532_packetbuffer, 3)) {
#ifdef PN532DEBUG
        PN532DEBUGPRINT.println(F("Initiate failed"));
#endif
        return false;
    }

    // Read response
    readData(pn532_packetbuffer, 10);

    // 7 = Skip a response byte when using I2C to ignore extra data.
    if (pn532_packetbuffer[7] != 0x00) {
#ifdef PN532DEBUG
        PN532DEBUGPRINT.println(F("Initiate packet different than zero"));
#endif
        return false;
    }

    chip_id = pn532_packetbuffer[8];

    /*** SELECT ***/
    // Send SRIX4K command SELECT (0x0e chip_id)
    pn532_packetbuffer[0] = PN532_COMMAND_INCOMMUNICATETHRU;
    pn532_packetbuffer[1] = SRIX4K_SELECT;
    pn532_packetbuffer[2] = chip_id;
    if (!sendCommandCheckAck(pn532_packetbuffer, 3)) {
#ifdef PN532DEBUG
        PN532DEBUGPRINT.println(F("Select failed"));
#endif
        return false;
    }

    readData(pn532_packetbuffer, 10);
    if (pn532_packetbuffer[7] != 0x00) {
#ifdef PN532DEBUG
        PN532DEBUGPRINT.println(F("Select packet different than zero"));
#endif
        return false;
    }

    return true; // success
}

/**
 * Read a specified block from SRIX4K
 * @param address Number of block to read
 * @param block pointer to an array to save read block
 * @return success. If it's false there is an error.
 */
bool Arduino_PN532_SRIX::SRIX_read_block(uint8_t address, uint8_t *block) {
    // Send command READ_BLOCK Address
    pn532_packetbuffer[0] = PN532_COMMAND_INCOMMUNICATETHRU;
    pn532_packetbuffer[1] = SRIX4K_READBLOCK;
    pn532_packetbuffer[2] = address;

    if (!sendCommandCheckAck(pn532_packetbuffer, 3)) {
#ifdef PN532DEBUG
        PN532DEBUGPRINT.println(F("READ_BLOCK failed"));
#endif
        return false;
    }

    // Read response
    readData(pn532_packetbuffer, 13);

    // Check response
    if (pn532_packetbuffer[7] != 0x00) {
#ifdef PN532DEBUG
        PN532DEBUGPRINT.println(F("READ_BLOCK first byte is different than 0x00"));
#endif
        return false;
    }

    // Save result
    block[0] = pn532_packetbuffer[8];
    block[1] = pn532_packetbuffer[9];
    block[2] = pn532_packetbuffer[10];
    block[3] = pn532_packetbuffer[11];

    return true;
}

/**
 * Write a specific block to SRIX4K
 * @param address Number of block to overwrite
 * @param block data to write (array of length 4)
 * @return success. If it's false there is an error.
 */
bool Arduino_PN532_SRIX::SRIX_write_block(uint8_t address, uint8_t *block) {
    // Send command WRITE_BLOCK Address Value
    pn532_packetbuffer[0] = PN532_COMMAND_INCOMMUNICATETHRU;
    pn532_packetbuffer[1] = SRIX4K_WRITEBLOCK;
    pn532_packetbuffer[2] = address;
    pn532_packetbuffer[3] = block[0];
    pn532_packetbuffer[4] = block[1];
    pn532_packetbuffer[5] = block[2];
    pn532_packetbuffer[6] = block[3];

    if (!sendCommandCheckAck(pn532_packetbuffer, 7)) {
#ifdef PN532DEBUG
        PN532DEBUGPRINT.println(F("WRITE_BLOCK failed"));
#endif
        return false;
    }

    return true;
}

/**
 * Get UID from SRIX4K
 * @param buffer Pointer to an array to save uid (min length: 8)
 * @return success. If it's false there is an error.
 */
bool Arduino_PN532_SRIX::SRIX_get_uid(uint8_t *buffer) {
    // Send command GET_UID
    pn532_packetbuffer[0] = PN532_COMMAND_INCOMMUNICATETHRU;
    pn532_packetbuffer[1] = SRIX4K_GETUID;
    if (!sendCommandCheckAck(pn532_packetbuffer, 2)) {
#ifdef PN532DEBUG
        PN532DEBUGPRINT.println(F("GET_UID failed"));
#endif
        return false;
    }

    // read i2c data to buffer
    readData(pn532_packetbuffer, 16);

    // Check first byte
    if (pn532_packetbuffer[7] != 0x00) {
#ifdef PN532DEBUG
        PN532DEBUGPRINT.println(F("Error during GET_UID command: first byte different than 0"));
#endif
        return false;
    }

    // Save UID data to buffer
    buffer[0] = pn532_packetbuffer[8];
    buffer[1] = pn532_packetbuffer[9];
    buffer[2] = pn532_packetbuffer[10];
    buffer[3] = pn532_packetbuffer[11];
    buffer[4] = pn532_packetbuffer[12];
    buffer[5] = pn532_packetbuffer[13];
    buffer[6] = pn532_packetbuffer[14];
    buffer[7] = pn532_packetbuffer[15];

    return true;
}
