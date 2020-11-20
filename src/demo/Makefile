# --------------------------------------------------------------------------
# SN74HC595 based debugger for the PIC12F675
#
# Author: Bernhard Bablok
#
# https://github.com/bablokb/pic-toolchain
# 
# --------------------------------------------------------------------------

PIN_SR_CLK=1
PIN_SR_DATA=2
PIN_BTN=5
LENGTH=BYTE

PROJECT=sn74hc595_dbg
EXTRA_C=shift_lib.c
EXTRA_ASM=delay.asm
MODEL=12f675
ARCH=pic14
SDCC_ARGS=-DSO_$(LENGTH) -DPIN_SR_CLK=$(PIN_SR_CLK) -DPIN_SR_DATA=$(PIN_SR_DATA) \
                    -DPIN_BTN=$(PIN_BTN)

include rules.inc

# --------------------------------------------------------------------------
