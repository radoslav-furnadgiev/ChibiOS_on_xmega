# List of all the board related files.
BOARDSRC = ${PROJECTDIR}/boards/ATMEL_XPLAIN/board.c

# Required include directories
BOARDINC = ${PROJECTDIR}/boards/ATMEL_XPLAIN

#Board specific build flags
CFLAGS += -mmcu=atxmega128a1
LDFLAGS += -mmcu=atxmega128a1
