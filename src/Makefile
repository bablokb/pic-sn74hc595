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

PROJECT=sn74hc595_dbg
EXTRA_C=shift_lib.c
EXTRA_ASM=delay.asm
MODEL=12f675
ARCH=pic14
SDCC_ARGS=-DSO_BYTE -DPIN_SR_CLK=$(PIN_SR_CLK) -DPIN_SR_DATA=$(PIN_SR_DATA)

include rules.inc

# --------------------------------------------------------------------------
