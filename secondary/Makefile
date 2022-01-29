DEVICE  		= attiny85
F_CPU   		= 16500000	# in Hz
FUSE_L  		= 0xE1
FUSE_H  		= 0xDD
AVRDUDE 		= avrdude -c avrisp2 -p $(DEVICE)

CFLAGS  		= -std=c99 -Wall -g -Os -mmcu=${DEVICE} -DF_CPU=${F_CPU} -I.
OBJECTS 		= main.o

TARGET_BUTTON 	= main_secondary_button
SRC_BUTTON   	= main_secondary_button.c

TARGET_LEDS 	= main_secondary_leds
SRC_LEDS   		= main_secondary_leds.c

CC 	   			= avr-gcc
OBJCOPY			= avr-objcopy

# symbolic targets:
help:
	@echo "This Makefile has no default rule. Use one of the following:"

# rule for programming fuse bits:
fuse:
	@[ "$(FUSE_H)" != "" -a "$(FUSE_L)" != "" ] || \
		{ echo "*** Edit Makefile and choose values for FUSE_L and FUSE_H!"; exit 1; }
	$(AVRDUDE) -U hfuse:w:$(FUSE_H):m -U lfuse:w:$(FUSE_L):m

all:
		${CC} ${CFLAGS} -o ${TARGET_BUTTON}.bin ${SRC_BUTTON}
		${OBJCOPY} -j .text -j .data -O ihex ${TARGET_BUTTON}.bin ${TARGET_BUTTON}.hex

		${CC} ${CFLAGS} -o ${TARGET_LEDS}.bin ${SRC_LEDS}
		${OBJCOPY} -j .text -j .data -O ihex ${TARGET_LEDS}.bin ${TARGET_LEDS}.hex

flash_button:
		$(AVRDUDE) -U flash:w:${TARGET_BUTTON}.hex:i -P usb

flash_leds:
		$(AVRDUDE) -U flash:w:${TARGET_LEDS}.hex:i -P usb

clean:
		rm -f *.bin *.hex

