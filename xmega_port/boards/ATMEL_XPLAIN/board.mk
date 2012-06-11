# List of all the board related files.
BOARDSRC = ${BOARDDIR}/board.c

# Required include directories
BOARDINC = ${BOARDDIR}

#Board specific build flags
CFLAGS += -mmcu=atxmega128a1
LDFLAGS += -mmcu=atxmega128a1
