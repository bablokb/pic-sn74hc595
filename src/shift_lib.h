// --------------------------------------------------------------------------
// SN74HC595 driver
//
// Author: Bernhard Bablok
// https://github.com/bablokb/pic-sn74hc595
//
// --------------------------------------------------------------------------

#ifndef __SHIFT_LIB_H
  #define __SHIFT_LIB_H

  #include "picconfig.h"
  #include <stdint.h>

// Define Pins
#ifndef PIN_SR_CLK
  #define PIN_SR_CLK 1
#endif
#define GP_CLK     _CONCAT(GP,PIN_SR_CLK)      // Serial clock pin

#ifndef PIN_SR_DATA
  #define PIN_SR_DATA 2
#endif
#define GP_DATA    _CONCAT(GP,PIN_SR_DATA)     // Serial data pin

  // Function Declarations
  void so_init(void);
  #ifdef SO_BYTE
    void so_byte(uint8_t data);
    #define so_clear() so_byte(0x00)
  #endif
  #ifdef SO_ADDR
    void so_addr(uint16_t data);
    #define so_clear() so_addr(0x00)
  #endif
#endif
