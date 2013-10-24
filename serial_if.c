#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "libarduino.h"
#include "serial_if.h"

//#define CRC_CHECK

#define TRANSACTION_SIZE	4

static uint8_t serial_buffer[TRANSACTION_SIZE];
static volatile uint8_t writeptr;

ISR(SERIAL_IF0_ISR_VECT)
{
	if (writeptr < TRANSACTION_SIZE) {	
		serial_buffer[writeptr] = SERIAL_IF0_DATA;
		writeptr++;
	}
}

void serial_init(void)
{
	writeptr = 0;
	
	// set default baud rate 
	UBRR0H = SERIAL_IF_BAUD_SELECT >> 8;  
	UBRR0L = SERIAL_IF_BAUD_SELECT;

	// enable receive, transmit and ensable receive interrupts 
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);

	/* dont forget sei() */
}

/* Return -1 if fails, 0 if success                */
int get_transaction(transaction_t * transaction)
{
	uint8_t crc;
	int result;
	
	if (transaction == NULL) return E_SYS;
	if (writeptr < TRANSACTION_SIZE) return E_NOT_READY;	// transaction still in progress
	
	transaction->cmd = (serial_buffer[0] & CMD_MASK) >> 4;
	transaction->addr = serial_buffer[0] & ADDR_MASK;
	transaction->value = (serial_buffer[1] << 8) | serial_buffer[2];
	transaction->crc = serial_buffer[3];

#ifdef CRC_CHECK	
	crc = crc8(serial_buffer, 3);
	if (crc == transaction->crc) {		/* CRC OK ? */
		result = E_OK;
	} else {
		result = E_CRC;
	}	
#else
	result = E_OK;
#endif	
	writeptr = 0;
	
	return result;
}


int send_transaction(transaction_t * transaction)
{   
	uint8_t serialized[4];
	int i;
	
	if (transaction == NULL) return E_SYS;
	
	serialized[0] = (transaction->cmd << 4) | (transaction->addr & ADDR_MASK);
	serialized[1] = (transaction->value >> 8) & 0xff;
	serialized[2] = transaction->value & 0xff;
	serialized[3] = crc8(serialized, 3);
	
	for (i=0; i<4; i++) {
		while ( !(UCSR0A & (1<<UDRE0)) ); /* block till we have space */
		SERIAL_IF0_DATA = serialized[i]; /* send */
	}
	
	return E_OK;
}

