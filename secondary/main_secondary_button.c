#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

#include "../i2c/i2c_secondary.c"

#define PIN_BUTTON			PB1
#define PIN_BUTTON_LED		PB3

#define ADDRESS_BUTTON 		0xBA

#define READ_BUFFER_SIZE 	5

unsigned char read_buffer[READ_BUFFER_SIZE];

int main() {

	DDRB |= (1 << PIN_BUTTON_LED);
	PORTB |= (1 << PIN_BUTTON); // pull up on button

	for (int i = 0; i < READ_BUFFER_SIZE; i++) {
		read_buffer[i] = 0x00;
	}

	// init i2c
	i2c_secondary_init(ADDRESS_BUTTON, read_buffer, READ_BUFFER_SIZE);

	sei();

	while(1) {

		// set buffer (READ from primary)
 		if (!(PINB & (1 << PIN_BUTTON))) { // button pressed
			PORTB |= (1 << PIN_BUTTON_LED);
			read_buffer[0] = 0x23; // arbitary information just for testing
			read_buffer[1] = 0x45; // (there's more than one byte to test multiple
			read_buffer[2] = 0x67; // read operations on the primary side)
		} else {
			PORTB &= ~(1 << PIN_BUTTON_LED);
			read_buffer[0] = 0x00;
			read_buffer[1] = 0x00;
			read_buffer[2] = 0x00;
		}


	}
}