#include "i2c_secondary.h"

void i2c_secondary_init(unsigned char address, unsigned char *read_buffer, unsigned int read_buffer_size) {

	i2c_secondary_address = address;
	i2c_read_buffer = read_buffer;
	i2c_read_buffer_size = read_buffer_size;

	USICR = USICR_START_STATUS;
	USISR = USISR_START_STATUS;

	DDRB &= ~(1 << PIN_SDA); // input
	DDRB &= ~(1 << PIN_SCL); // input

}

void i2c_secondary_reset() {
	i2c_secondary_status = I2C_STATUS_WAITING_START;
	USICR = USICR_START_STATUS;
	USISR = USISR_START_STATUS;

	DDRB &= ~(1 << PIN_SDA); // input
	DDRB &= ~(1 << PIN_SCL); // input
}


void i2c_secondary_send_nack(int next_status, int status_after_nack, int USISR_after_nack) {
	USIDR = 0;
	DDRB |= (1 << PIN_SDA); // output to send the nack (auto-low when changing)
	USISR = USISR_CLOCK_1_BIT;

	i2c_secondary_status = next_status;
	i2c_secondary_status_after_nack = status_after_nack;
	i2c_secondary_USISR_after_nack = USISR_after_nack;
}

void i2c_secondary_set_waiting_read() {

	// SDA as output
	DDRB |= (1 << PIN_SDA);
	PORTB |= (1 << PIN_SDA); // also release SDA

	if (i2c_read_buffer_index < i2c_read_buffer_size) {
		USIDR = i2c_read_buffer[i2c_read_buffer_index++];
	} else {
		// send 0x00 when overflow
		USIDR = 0x00;
	}

	USISR = USISR_CLOCK_8_BITS;

	i2c_secondary_status = I2C_STATUS_PREPARE_RECIVING_ACK;
}

ISR(USI_START_vect) {

	// wait for start condition to finish
	while( (PINB & (1 << PIN_SCL)) && (!(PINB & (1 << PIN_SDA))) );

	USICR = USICR_START_STATUS | (1 << USIOIE); //  enable overflow interrupt
	i2c_secondary_status = I2C_STATUS_WAITING_ADDRESS;

	// reset flags
	USISR = USISR_START_STATUS;

}

ISR(USI_OVF_vect) {

	switch(i2c_secondary_status) {
		// case I2C_STATUS_WAITING_START: // cannot happen
		case I2C_STATUS_WAITING_ADDRESS:

			if ( (USIDR & 0xFE) == i2c_secondary_address) { // ignore r/w last bit
				if (USIDR & 0x01) {
					i2c_read_buffer_index = 0; // each write starts at the beginning of the buffer
					i2c_secondary_send_nack(I2C_STATUS_WAITING_READ, 0, 0);
				} else i2c_secondary_send_nack(I2C_STATUS_WAITING_SENDING_NACK, I2C_STATUS_WAITING_WRITE, USISR_CLOCK_8_BITS);
			} else {
				i2c_secondary_reset(); // not our address
			}
			break;

		case I2C_STATUS_WAITING_WRITE:

			i2c_command = USIDR;
			i2c_secondary_send_nack(I2C_STATUS_WAITING_SENDING_NACK, I2C_STATUS_WAITING_START, USISR_START_STATUS);
			break;

		case I2C_STATUS_WAITING_READ:

			// put the data on USIDR and prepare SDA for sending it over 8 clocks
			i2c_secondary_set_waiting_read();
			break;

		case I2C_STATUS_WAITING_SENDING_NACK:

			DDRB &= ~(1 << PIN_SDA); // input, again
			USISR = i2c_secondary_USISR_after_nack; // next USISR status will depend on who call it (start condition interrupt will be set to 1 again only when finishing a write)
			i2c_secondary_status = i2c_secondary_status_after_nack;
			break;

		case I2C_STATUS_PREPARE_RECIVING_ACK:

			PORTB &= ~(1 << PIN_SDA); // SDA port off just in case before setting it on input
			// SDA as input again
			DDRB &= ~(1 << PIN_SDA);

			i2c_secondary_status = I2C_STATUS_WAITING_RECIVING_ACK;
			USISR = USISR_CLOCK_1_BIT; // on the next clock tick we'll have the ack from controller

			break;
		case I2C_STATUS_WAITING_RECIVING_ACK:

			if (USIDR & 0x01) { // that's a NACK, we're done
				i2c_secondary_reset();
			} else {
				i2c_secondary_set_waiting_read();
			}

			break;
	} 

}