/*
 * LCD1602A.h - LCD1602/LCD2004 over PCF8574 I2C backpack.
 *
 * Target: micro:bit V2 (nRF52833) with Mind+ upload mode.
 * Uses the Mind+ <Wire.h> library constructed on the edge connector pins
 *   SDA = P20, SCL = P19
 * (verified on this core: TwoWire(P20, P19) + begin() + 0x27 ACK).
 *
 * Copyright (c) zcode. MIT License.
 */
#ifndef LCD1602A_H
#define LCD1602A_H

#include "MicroBit.h"
#include <Wire.h>

// PCF8574 backpack bit map: P0=RS P1=RW P2=EN P3=BL P4..P7=D4..D7
#define LCD_BL      0x08

class LCD1602A {
public:
  LCD1602A(uint8_t addr = 0x27, uint8_t cols = 16, uint8_t rows = 2);
  ~LCD1602A();

  bool begin();
  void clear();
  void print(const char *text);
  void setCursor(uint8_t col, uint8_t row);
  void backlight(bool on);
  void command(uint8_t cmd);
  bool isReady() const { return ready; }

private:
  void pulse(uint8_t nibble, uint8_t rs);
  void send(uint8_t value, bool isData);
  void raw(uint8_t v);

  uint8_t _addr;
  uint8_t _cols;
  uint8_t _rows;
  uint8_t _bl;
  bool ready;
  TwoWire *wire;
};

#endif
