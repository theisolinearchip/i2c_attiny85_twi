/*
	Raw library for ssd1306 oled-like small screens. This one
	handles	single and multiple command transactions, even with data
	from PROGMEM (mostly used for displaying imaged stored there).

	I made this for my Game of Life on an attiny85 project:
	https://github.com/theisolinearchip/gameoflife_attiny85
	https://hackaday.io/project/181421-game-of-life-on-an-attiny85

	It's the same version but without some extra methods for a small
	matrix system I made in order to display the dynamic game with
	the small SRAM available on the attiny85 (not really relevant
	for the i2c part)

	It was intended to be used with a bit-banging implementation
	for the i2c protocol I made too (same functions, different code;
	changing the base-lib doesn't matter: it also works fine! :D)
*/

#ifndef SSD1306_attiny85_c
#define SSD1306_attiny85_c

#include "ssd1306_attiny85_constants.h"

void ssd1306_send_single_command(char command) {
	i2c_start();
	i2c_write_byte(SSD1306_I2C_WRITE_ADDRESS);
	i2c_write_byte(SSD1306_CONTROL_BYTE_ONE_COMMAND);
	i2c_write_byte(command);
	i2c_stop(); 
}

void ssd1306_init() {
	i2c_start();
	i2c_write_byte(SSD1306_I2C_WRITE_ADDRESS);
	for (int i = 0; i < 29; i++) {
		i2c_write_byte(SSD1306_CONTROL_BYTE_MULTIPLE_COMMANDS);
		i2c_write_byte(pgm_read_byte(init_options + i)); // progmem access
	}
	i2c_stop();
}

void ssd1306_send_multiple_commands(int length, char commands[]) {
	if (length <= 0) return;

	i2c_start();
	i2c_write_byte(SSD1306_I2C_WRITE_ADDRESS);
	for (int i = 0; i < length; i++) {
		i2c_write_byte(SSD1306_CONTROL_BYTE_MULTIPLE_COMMANDS);
		i2c_write_byte(commands[i]);
	}
	i2c_stop();
}

void ssd1306_send_single_data(char data) {
	i2c_start();
	i2c_write_byte(SSD1306_I2C_WRITE_ADDRESS);
	i2c_write_byte(SSD1306_CONTROL_BYTE_ONE_DATA);
	i2c_write_byte(data);
	i2c_stop(); 
}

void ssd1306_send_multiple_data(int length, char data[]) {
	i2c_start();
	i2c_write_byte(SSD1306_I2C_WRITE_ADDRESS);
	for (int i = 0; i < length; i++) {
		i2c_write_byte(SSD1306_CONTROL_BYTE_MULTIPLE_DATA);
		i2c_write_byte(data[i]);
	}
	i2c_stop();
}

void ssd1306_send_progmem_multiple_data(const int length, const char *data) {
	i2c_start();
	i2c_write_byte(SSD1306_I2C_WRITE_ADDRESS);
	for (int i = 0; i < length; i++) {
		i2c_write_byte(SSD1306_CONTROL_BYTE_MULTIPLE_DATA);
		i2c_write_byte(pgm_read_byte(data + i));
	}
	i2c_stop();
}

#endif