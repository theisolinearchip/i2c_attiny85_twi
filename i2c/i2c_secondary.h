
#define PIN_SDA 	PB0
#define PIN_SCL 	PB2

#define USICR_START_STATUS 0b10101000 // (1 << USISIE) | (1 << USIWM1) | (1 << USICS1) // start interrupt activated, two-wire mode, external clock (from controller device)
#define USISR_START_STATUS 0b11110000 // (1 << USISIF) | (1 << USIOIF) | (1 << USIPF) | (1 << USIDC); // reset all the flags

#define USISR_CLOCK_1_BIT  0b01111110 // used for nack
#define USISR_CLOCK_8_BITS 0b01110000 // used for sending data to primary device (8 bits)

#define I2C_STATUS_WAITING_START 			1 	// waiting for a start condition interrupt
#define I2C_STATUS_WAITING_ADDRESS 			2 	// once the start condition has started the first 8 bits will be the address, managed here
#define I2C_STATUS_WAITING_WRITE 			3 	// if it's a write address, the next 8 bits will be processed here (usually some "command" to process)
#define I2C_STATUS_WAITING_READ	 			4 	// if it's a read, here we'll put 8 bits to be sent to the primary one
#define I2C_STATUS_WAITING_SENDING_NACK	 	5 	// after the secondary sends a nack it'll end up here to be re-routed to the next step (start again, go to read, go to write...)
#define I2C_STATUS_PREPARE_RECIVING_ACK 	6 	// after sending 8 bits the secondary will prepare itself to recieve a single (n)ack here
#define I2C_STATUS_WAITING_RECIVING_ACK 	7 	// after the (n)ack is sent from the primary one after a read operation it's value will be managed here (1/0: stop sending / continue)

volatile unsigned int i2c_secondary_status = I2C_STATUS_WAITING_START;
volatile unsigned int i2c_secondary_status_after_nack = I2C_STATUS_WAITING_START;
volatile unsigned int i2c_secondary_USISR_after_nack = USICR_START_STATUS;

volatile unsigned int i2c_command = 0x00;

volatile unsigned char i2c_secondary_address = 0x00; // 8 bits address, 1/0 on bit 0 depends on sending_data / reciving_data

volatile unsigned char *i2c_read_buffer;
volatile unsigned int i2c_read_buffer_index = 0;
volatile unsigned int i2c_read_buffer_size = 0;