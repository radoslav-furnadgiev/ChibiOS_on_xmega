# List of all the board related files.
BOARDSRC = ${BOARDDIR}/board.c

# Required include directories
BOARDINC = ${BOARDDIR}

#Board specific build flags
MCU=atxmega128a1
CFLAGS += -mmcu=$(MCU)
LDFLAGS += -mmcu=$(MCU)
