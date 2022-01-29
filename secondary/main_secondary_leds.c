#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

#include "../i2c/i2c_secondary.c"

#define PIN_LED_1			PB1
#define PIN_LED_2			PB3
#define PIN_LED_3			PB4

#define ADDRESS_LEDS	  	0xAA

#define READ_BUFFER_SIZE 	5

unsigned char read_buffer[READ_BUFFER_SIZE];  // not used here

int main() {

 	DDRB |= (1 << PIN_LED_1);
 	DDRB |= (1 << PIN_LED_2);
 	DDRB |= (1 << PIN_LED_3);

 	// start-up sequence
	PORTB |= (1 << PIN_LED_1);
	_delay_ms(100);
	PORTB &= ~(1 << PIN_LED_1);
	PORTB |= (1 << PIN_LED_2);
	_delay_ms(100);
	PORTB &= ~(1 << PIN_LED_2);
	PORTB |= (1 << PIN_LED_3);
	_delay_ms(100);
	PORTB &= ~(1 << PIN_LED_3);

	// init i2c (buffer not used here)
	i2c_secondary_init(ADDRESS_LEDS, read_buffer, READ_BUFFER_SIZE);

	sei();

	while(1) {

		// process command (primary WRITES here with some
		// commands to work with - some simple examples here)
		if (i2c_command != 0x00) {
			switch(i2c_command) {
				case 0x11: // 0x11: "turn led 2 on"
					PORTB |= (1 << PIN_LED_2);
					break;
				case 0x22: // 0x22: "turn led 2 off"
					PORTB &= ~(1 << PIN_LED_2);
					break;
			}

			// command reset after dealing with it
			i2c_command = 0x00;
		}

	}
}