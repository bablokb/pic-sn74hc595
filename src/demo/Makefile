# --------------------------------------------------------------------------
# Demo-program for the SN74HC595
#
# Author: Bernhard Bablok
#
# https://github.com/bablokb/pic-sn74hc595
# 
# --------------------------------------------------------------------------

PIN_SR_CLK=2
PIN_SR_DATA=1
PIN_BTN=5
OTYPE=BYTE
#OTYPE=ADDR


PROJECT=sn74hc595_demo
EXTRA_C=shift_lib.c
EXTRA_ASM=delay.asm
ARCH=pic14
SDCC_ARGS=-DSO_$(OTYPE) -DPIN_SR_CLK=$(PIN_SR_CLK) \
                        -DPIN_SR_DATA=$(PIN_SR_DATA) \
                        -DPIN_BTN=$(PIN_BTN)

include ptc_rules.inc

# --------------------------------------------------------------------------
