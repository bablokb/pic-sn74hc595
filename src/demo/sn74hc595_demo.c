// --------------------------------------------------------------------------
// Demo-program for the SN74HC595
//
// A simple SN74HC595-based "debugger" which only uses two pins to shift-out
// 8 or 16 bits. Connected to the SN74HC595 are LEDs to display the data.
//
// The sample program uses a button on GP5 (configurable with PIN_BTN).
// After intitialization, the leds should display '0000 1111'.
// The button toggles a counter showing all values from 1 to 255. When
// pressed again or when the counter wraps around to zero, the counting
// stops again.
//
// If you chain two SN74HC595 together and #define SO_ADDR instead of
// SO_BYTE, the sample program will output ~counter,counter.
//
// You can find a pcb for the shift-register in the sn74hc595.kicad
// directory of this project.
//
// Author: Bernhard Bablok
// https://github.com/bablokb/pic-sn74hc595
//
// --------------------------------------------------------------------------

#include "picconfig.h"
#include <stdint.h>

#include "shift_lib.h"
#include "delay.h"

#ifndef PIN_BTN
  #define PIN_BTN 5
#endif
#define GP_BTN _CONCAT(GP,PIN_BTN)

static union {
  uint16_t addr;
  struct {
    uint8_t byte1;
    uint8_t byte2;
  } counter;
} d;

CONFIG_WORDS

////////////////////////////////////////////////////////////////////////
// Intialize registers

static void init(void) {
  // configure registers

  __asm__ ("CLRWDT");            // clear WDT even if WDT is disabled
  ANSEL  = 0;                    // no analog input
  CMCON  = 0x07;                 // disable comparator for GP0-GP2
  TRISIO = 1 << PIN_BTN;         // GP_BTN is input
  WPU    = 1 << PIN_BTN;         // weak pullups enable on GP_BTN
  IOC    = 1 << PIN_BTN;         // IOC on GP_BTN

  NOT_GPPU = 0;   // enable pullups
  GPIO     = 0;
  INTCON   = 0;   // clear interrupt flag bits
  GPIE     = 1;   // enable IOC

  INIT_SPECIAL;
  CLOCK_4MHZ;
}

////////////////////////////////////////////////////////////////////////
// Interrupt service routine

static void isr(void) __interrupt 0 {
  if (GPIF) {                  // interrupt-on-change
    // just wait a bit to debounce
    delay_ms(250);
    if (d.counter.byte1) {   // active: stop counting
      d.counter.byte1 = 0;
    } else {         // waiting: start counting
      d.counter.byte1 = 1;
      d.counter.byte2 = ~d.counter.byte1;
    }
#ifdef __12F1612
    IOCAF = 0;                // clear IOC interrupt flag
#else
    GPIF = 0;                  // clear IOC interrupt flag
#endif
  }
}

// --- main program   --------------------------------------------------------

void main(void) {
  d.addr = 0;
  init();
  so_init();

#ifdef SO_BYTE
  so_byte(0x0F);      // 0000 1111
#else
  so_addr(0xF00F);    // 1111 0000 0000 1111
#endif

  GIE = 1;   // global interrupt enable
  while (1) {
    // if d.counter.byte1 is zero, wait for button-press and ...
    while (!d.counter.byte1) {
      __asm__("SLEEP");
    }

    // ... display and increment and wait some time
    GIE = 0;   // global interrupt disable
#ifdef SO_BYTE
    so_byte(d.counter.byte1++);
#else
    so_addr(d.addr);
    d.counter.byte1++;
    d.counter.byte2 = ~d.counter.byte1;
#endif
    GIE = 1;   // global interrupt enable
    delay_ms(250);
  }
}
