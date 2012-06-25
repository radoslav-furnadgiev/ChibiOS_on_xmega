CHIBIOS     = ./ChibiOS
PORTDIR     = ./xmega_port/os/ports/GCC/AVRXMEGA
BOARDDIR    = ./xmega_port/boards/ATMEL_XPLAIN
PLATFORMDIR = ./xmega_port/os/hal/platforms/AVRXMEGA
PROJECTDIR  = ./xmega_demo

TARGET      = xmega_demo

CFLAGS      = -fomit-frame-pointer -Os -ggdb -Wl,-static -fdata-sections -ffunction-sections -DF_CPU=32000000 -Wall
LDFLAGS     = -Wl,--gc-sections

include $(CHIBIOS)/os/kernel/kernel.mk
include $(CHIBIOS)/os/hal/hal.mk
include $(PLATFORMDIR)/platform.mk
include $(PORTDIR)/port.mk
include $(BOARDDIR)/board.mk
include $(CHIBIOS)/test/test.mk
include $(PROJECTDIR)/project.mk

SOURCES =   $(KERNSRC) \
            $(HALSRC) \
            $(PLATFORMSRC) \
            $(PORTSRC) \
            $(BOARDSRC) \
            $(TESTSRC) \
            $(PROJECTSRC)

INCDIRS =   -I$(KERNINC) \
            -I$(HALINC) \
            -I$(PLATFORMINC) \
            -I$(PORTINC) \
            -I$(BOARDINC) \
            -I$(TESTINC) \
            -I$(PROJECTINC)

OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET).elf

$(TARGET).elf: $(OBJECTS)
	avr-gcc $(LDFLAGS) -o $@ $(OBJECTS)
	avr-size --format=avr --mcu=$(MCU) $(TARGET).elf

%.o: %.c
	avr-gcc $(CFLAGS) $(INCDIRS) -c -o $@ $<


.PHONY: clean

clean:
	rm -rf $(OBJECTS)
	rm -rf $(TARGET).elf
