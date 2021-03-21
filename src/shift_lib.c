// --------------------------------------------------------------------------
// SN74HC595 driver
//
// Author: Bernhard Bablok
// https://github.com/bablokb/pic-sn74hc595
//
// --------------------------------------------------------------------------

#include "shift_lib.h"

// --------------------------------------------------------------------------
// setup hardware for pins

void so_init() {
  TRISA   &= ~(1<<PIN_SR_DATA);
  TRISA   &= ~(1<<PIN_SR_CLK);
  GP_DATA  = 0;
  GP_CLK   = 0;
}

// --------------------------------------------------------------------------
// Shift out 8+1 bits to shift-register IC

#ifdef SO_BYTE
void so_byte(uint8_t data) {
  uint8_t i = 0;

  // write data to shift-register
  for (i=7;i<255;i--) {
    GP_DATA = ((data>>i)&0x1);   // write bit value
    GP_CLK  = 1;                 // toggle clock pin
    GP_CLK  = 0;
  }
  GP_CLK  = 1;                   // toggle clock pin
  GP_CLK  = 0;
}
#endif

// --------------------------------------------------------------------------
// Shift out 16+1 bits to shift-register IC
// This function needs two SN74HC595-ICs chained together.

#ifdef SO_ADDR
void so_addr(uint16_t data) {
  uint8_t i = 0;

  // write data to shift-register
  for (i=16;i<255;i--) {
    GP_DATA = ((data>>i)&0x1);   // write bit value
    GP_CLK  = 1;                 // toggle clock pin
    GP_CLK  = 0;
  }
  GP_CLK  = 1;                   // toggle clock pin
  GP_CLK  = 0;
}
#endif
