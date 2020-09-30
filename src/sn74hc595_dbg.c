// --------------------------------------------------------------------------
// SN74HC595 based debugger for the PIC12-family
//
// A simple SN74HC595-based "debugger" which only uses two pins to shift-out
// 8 or 16 bits. Connected to the SN74HC595 are LEDs to display the data.
//
// The sample program uses a button on GP5. After intitialization,
// the leds should display '0000 1111'. The button toggles a counter showing
// all values from 1 to 255. When pressed again or when the counter wraps
// around to zero, the counting stops again.
//
// If you chain two SN74HC595 together and #define SO_ADDR instead of
// SO_BYTE, the sample program will output ~counter,counter.
//
// Author: Bernhard Bablok
// https://github.com/bablokb/pic-sn74hc595
//
// --------------------------------------------------------------------------

#include <pic14regs.h>
#include <stdint.h>

#include "shift_lib.h"
#include "delay.h"

// build with:
// make build

static union {
  uint16_t addr;
  struct {
    uint8_t byte1;
    uint8_t byte2;
  } counter;
} d;

#ifdef __SDCC_PIC12F675
  #undef RP0
#elif __SDCC_PIC12F683
  #define CMCON      CMCON0
  #define _BODEN_OFF _BOD_OFF
#elif __SDCC_PIC12F1840
  #define CMCON    CM1CON0
  #define ANSEL    ANSELA
  #define TRISIO   TRISA
  #define GPIO     PORTA
  #define GP5      RA5
  #define WPU      WPUA
  #define IOC      IOCAN
  #define GPIE     IOCIE
  #define GPIF     IOCIF
  #define NOT_GPPU NOT_WPUEN
 #endif

// MCLR on, Power on Timer, no WDT, int-oscillator, 
// no brown out

#if defined __SDCC_PIC12F675 || defined __SDCC_PIC12F683
__code uint16_t __at (_CONFIG) __configword = 
  _MCLRE_OFF & _PWRTE_ON & _WDT_OFF & _INTRC_OSC_NOCLKOUT & _BODEN_OFF;
#elif __SDCC_PIC12F1840
__code uint16_t __at (_CONFIG1) __configword1 =
  _MCLRE_OFF & _PWRTE_ON & _WDTE_OFF & _CLKOUTEN_OFF & _BOREN_OFF & _FOSC_INTOSC;
__code uint16_t __at (_CONFIG2) __configword2 = _LVP_OFF & _DEBUG_OFF;
#endif

////////////////////////////////////////////////////////////////////////
// Intialize registers

static void init(void) {
  // configure registers

#ifdef __SDCC_PIC12F1840
  OSCCONbits.IRCF = 0b1101;                 // run at 4MHz
#endif

  __asm__ ("CLRWDT");            // clear WDT even if WDT is disabled
  ANSEL  = 0;                    // no analog input
  CMCON  = 0x07;                 // disable comparator for GP0-GP2
  TRISIO = 0b100000;             // GP5 is input
  WPU    = 0b100000;             // weak pullups enable on GP5
  IOC    = 0b100000;             // IOC on GP5

  GP5      = 0;   // initial value of GP5

  NOT_GPPU = 0;   // enable pullups
  GPIO     = 0;
  INTCON   = 0;   // clear interrupt flag bits
  GPIE     = 1;   // enable IOC
  GIE      = 1;   // global interrupt enable
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
    }
    GPIF = 0;                  // clear IOC interrupt flag
#ifdef __SDCC_PIC12F1840
    IOCAF = 0;
#endif
  }
}

// --- main program   --------------------------------------------------------

void main(void) {
#ifdef __SDCC_PIC12F675
  // Load calibration
  __asm
    bsf  STATUS, RP0
    call 0x3ff    ; read value
    movwf OSCCAL  ; set  value
    bcf  STATUS, RP0
  __endasm;
#endif

  d.addr = 0;
  init();
  so_init();
#ifdef SO_BYTE
  so_byte(0x0F);      // 0000 1111
#else
  so_addr(0xF00F);    // 1111 0000 0000 1111
#endif
  while (1) {
    // if d.counter.byte1 is zero, wait for button-press and ...
    while (!d.counter.byte1) {
      __asm__("SLEEP");
    }
    // ... display and increment and wait some time
#ifdef SO_BYTE
    so_byte(d.counter.byte1++);
#else
    so_addr(d.addr);
    d.counter.byte1++;
    d.counter.byte2 = ~d.counter.byte1;
#endif
    delay_ms(250);
  }
}
