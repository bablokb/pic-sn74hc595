// --------------------------------------------------------------------------
// SN74HC595 based debugger for the PIC12-family
//
// Author: Bernhard Bablok
// https://github.com/bablokb/pic-sn74hc595
//
// --------------------------------------------------------------------------


#ifndef __SHIFT_LIB_H
  #define __SHIFT_LIB_H

  #include <pic14regs.h>
  #include <stdint.h>

// Define Pins
#ifndef PIN_SR_CLK
  #define PIN_SR_CLK 1
#endif
#ifndef PIN_SR_DATA
  #define PIN_SR_DATA 2
#endif

#ifdef _RA0
  #define GP     RA
  #define TRISIO TRISA
#endif

#define GP_CLK     _CONCAT(GP,PIN_SR_CLK)      // Serial clock pin
#define GP_DATA    _CONCAT(GP,PIN_SR_DATA)     // Serial data pin

// Define Pins direction registrers
#define GP_CLK_TRISIO    _CONCAT(TRISIO,PIN_SR_CLK)
#define GP_DATA_TRISIO   _CONCAT(TRISIO,PIN_SR_DATA)

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
