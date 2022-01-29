#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h> 
#include <avr/pgmspace.h> // ssd1306

#include "../i2c/i2c_primary.c"
#include "../libs/ssd1306_attiny85.c"

#define PIN_LED 					PB4

#define BUTTON_DEVICE_ADDRESS 		0xBA
#define BUTTON_DEVICE_LEDS	 		0xAA

void error_loop() {
	while(1) { // inifinte loop
		PORTB |= (1 << PIN_LED);
		_delay_ms(250);
		PORTB &= ~(1 << PIN_LED);
		_delay_ms(250);
	}
}

int main() {

	DDRB |= (1 << PIN_LED);

	// init sequence
	PORTB |= (1 << PIN_LED);
	_delay_ms(500);
	PORTB &= ~(1 << PIN_LED);
	_delay_ms(1500); // some min init

	i2c_init();

	ssd1306_init();
	ssd1306_send_progmem_multiple_data(default_image_length, image_1); // ssd1306 raw example: show an image
	// ssd1306_send_progmem_multiple_data(default_image_length, image_2);

	unsigned char read_byte = 0x00;
	unsigned int read_word = 0x00;

	unsigned char write_command = 0x11;
	unsigned char last_write_command = 0x11;

	while(1) {

		// READ to get the button state
		read_byte = 0x00;
		read_word = 0x00;
		i2c_start();
		// BUTTON DEVICE (0xBA for write, 0xBB for read)
		if (i2c_write_byte(BUTTON_DEVICE_ADDRESS | 0x01) & 0x01) { // read address
			error_loop();
		}

		// perform three read operations and store the first two bytes on read_word
		// and the last one on read_byte. It's a quick and raw example of multiple
		// read operations, so the content doesn't make any sense (it's just a fixed
		// bunch of numbers that must be exactly the ones we want in order to turn
		// ON the led - otherwise nothing will happen...)
		read_word = (i2c_read_byte(0x00) << 8);
		read_word |= i2c_read_byte(0x00);
		read_byte = i2c_read_byte(0xFF);

		i2c_stop();

		// WRITE the led (and also set our own led!)
		if (read_word == 0x2345 && read_byte == 0x67) { // values sent by the secondary device
			PORTB |= (1 << PIN_LED);
			write_command = 0x11; // set led on
		} else {
			PORTB &= ~(1 << PIN_LED);
			write_command = 0x22; // set led off
		}

		if (write_command != last_write_command) { // just send if there's a change

			i2c_start();
			// LED DEVICE (0xAA)
			if (i2c_write_byte(BUTTON_DEVICE_LEDS) & 0x01) { // write address
				error_loop();
			}
			if (i2c_write_byte(write_command) & 0x01) {
				error_loop(); // write
			}
			i2c_stop();

		}

		last_write_command = write_command;

		_delay_ms(5);
	}


}