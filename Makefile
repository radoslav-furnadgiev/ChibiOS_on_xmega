CHIBIOS     = ./ChibiOS
PORTDIR     = ./ports/GCC/AVRXMEGA
BOARDDIR    = ./boards/ATMEL_XPLAIN
PLATFORMDIR = ./hal/platforms/AVRXMEGA
PROJECTDIR  = .

TARGET      = Xtest

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

%.o: %.c
	avr-gcc $(CFLAGS) $(INCDIRS) -c -o $@ $<


.PHONY: clean

clean:
	rm -rf $(OBJECTS)
	rm -rf $(TARGET).elf
