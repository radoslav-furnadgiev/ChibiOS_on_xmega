CHIBIOS     = ./ChibiOS
PORTDIR     = ./ports/GCC/AVRXMEGA
BOARDDIR    = ./boards/ATMEL_XPLAIN
PLATFORMDIR = ./hal/platforms/AVRXMEGA
PROJECTDIR  = .

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
.PHONY: obj

obj: $(OBJECTS)
%.o: %.c
	avr-gcc -c -mmcu=$(MCU) $(INCDIRS) -o $@ $<


.PHONY: clean

clean:
	rm -rf $(OBJECTS)
