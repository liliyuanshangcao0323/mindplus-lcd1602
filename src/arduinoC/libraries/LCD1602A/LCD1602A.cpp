/*
 * LCD1602A.cpp - LCD1602/LCD2004 over PCF8574 I2C backpack.
 * micro:bit V2 edge connector: P20 = SDA, P19 = SCL.
 * Uses Mind+ <Wire.h> (TwoWire constructed on the edge pins).
 * Copyright (c) zcode. MIT License.
 */
#include "LCD1602A.h"

LCD1602A::LCD1602A(uint8_t addr, uint8_t cols, uint8_t rows) {
  _addr = addr;
  _cols = cols;
  _rows = rows;
  _bl = LCD_BL;
  ready = false;
  wire = NULL;
}


LCD1602A::~LCD1602A() {
  if (wire) {
    delete wire;
    wire = NULL;
  }
}


void LCD1602A::raw(uint8_t v) {
  wire->beginTransmission(_addr);
  wire->write(v);
  wire->endTransmission();
  fiber_sleep(1);
}


void LCD1602A::pulse(uint8_t nibble, uint8_t rs) {
  uint8_t base = nibble | rs | _bl;
  raw(base);                   // EN low
  raw(base | 0x04);            // EN high -> latch on falling edge below
  raw(base);                   // EN low
}


void LCD1602A::send(uint8_t value, bool isData) {
  uint8_t rs = isData ? 0x01 : 0x00;
  pulse(value & 0xF0, rs);
  pulse((value << 4) & 0xF0, rs);
}


void LCD1602A::command(uint8_t cmd) {
  send(cmd, false);
  if (cmd <= 0x03) fiber_sleep(3);
}


bool LCD1602A::begin() {
  // Bus on the edge connector: SDA = P20, SCL = P19
  wire = new TwoWire(MICROBIT_PIN_P20, MICROBIT_PIN_P19);
  wire->begin();

  fiber_sleep(60);

  // HD44780 4-bit init sequence
  _bl = 0;
  pulse(0x30, 0); fiber_sleep(5);
  pulse(0x30, 0); fiber_sleep(5);
  pulse(0x30, 0); fiber_sleep(1);
  pulse(0x20, 0); fiber_sleep(1);      // switch to 4-bit
  _bl = LCD_BL;

  command(0x28);                       // 4-bit, 2 lines, 5x8
  command(0x08);                       // display off
  command(0x01); fiber_sleep(3);       // clear
  command(0x06);                       // entry mode: increment
  command(0x0C);                       // display on, cursor off
  ready = true;
  return true;
}


void LCD1602A::clear() {
  command(0x01);
  fiber_sleep(3);
}


void LCD1602A::setCursor(uint8_t col, uint8_t row) {
  static const uint8_t rowOffsets[4] = {0x00, 0x40, 0x14, 0x54};
  if (row > 3) row = 3;
  command(0x80 | (rowOffsets[row] + col));
}


void LCD1602A::print(const char *text) {
  while (*text) {
    send((uint8_t)(*text), true);
    text++;
  }
}


void LCD1602A::backlight(bool on) {
  _bl = on ? LCD_BL : 0x00;
  raw(_bl);
}
