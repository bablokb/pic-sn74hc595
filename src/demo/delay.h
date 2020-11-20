// --------------------------------------------------------------------------
// SN74HC595 based debugger for the PIC12F675
//
// Declaration of delay-functions
//
// Author: Bernhard Bablok
//
// https://github.com/bablokb/pic-sn74hc595
//
// --------------------------------------------------------------------------

#ifndef __DELAY_H
#define __DELAY_H

#include <stdint.h>
void delay_ms(uint8_t count);
#endif
