DEVICE  		= attiny85
F_CPU   		= 16000000UL
FUSE_L  		= 0xF1
FUSE_H  		= 0xDD
AVRDUDE 		= avrdude -c avrisp2 -p $(DEVICE)

CFLAGS  		= -std=c99 -Wall -g -Os -mmcu=${DEVICE} -DF_CPU=${F_CPU} -I.
OBJECTS 		= main.o

TARGET 			= main
SRC   			= main_primary.c

CC 	   			= avr-gcc
OBJCOPY 		= avr-objcopy

# symbolic targets:
help:
	@echo "This Makefile has no default rule. Use one of the following:"

# rule for programming fuse bits:
fuse:
	@[ "$(FUSE_H)" != "" -a "$(FUSE_L)" != "" ] || \
		{ echo "*** Edit Makefile and choose values for FUSE_L and FUSE_H!"; exit 1; }
	$(AVRDUDE) -U hfuse:w:$(FUSE_H):m -U lfuse:w:$(FUSE_L):m

all:
		${CC} ${CFLAGS} -o ${TARGET}.bin ${SRC}
		${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex

flash:
		$(AVRDUDE) -U flash:w:${TARGET}.hex:i -P usb

clean:
		rm -f *.bin *.hex

