# Raspberry Pi 1 HAL Library (for kernel, bootloader, serial and SD-Card drivers */

lib_LIBRARIES = librpihal.a

librpihal_a_SOURCES = \
  hal_arm.S \
  hal_mailbox.c \
  hal_mmio.c \
  hal_cache.c
    
nobase_include_HEADERS = machine/cheviot_hal.h \
                         machine/board/hal_arm.h \
                         machine/board/hal_arm.i \
                         machine/board/hal_mailbox.h \
                         machine/board/hal_mmio.h

AM_CFLAGS = -O2 -std=c99 -g0 -mcpu=cortex-a72 -mfpu=vfpv3-d16 -I. -Wall
AM_CCASFLAGS = -mcpu=cortex-a72 -mfpu=vfpv3-d16 -r -I.

