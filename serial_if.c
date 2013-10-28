#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "libarduino.h"
#include "serial_if.h"

//#define CRC_CHECK

#define SOF					'a'
#define TRANSACTION_SIZE	(sizeof(transaction_t)+1)

static uint8_t serial_buffer[TRANSACTION_SIZE];
static volatile uint8_t writeptr;

ISR(SERIAL_IF0_ISR_VECT)
{
	uint8_t data;
	
	if (writeptr < TRANSACTION_SIZE) {	
		data = SERIAL_IF0_DATA;
		if ((writeptr != 0) || (data == SOF)) {	// wait for the start byte
			serial_buffer[writeptr] = data;
			writeptr++;
		}
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
	sei();
}

/* Return -1 if fails, 0 if success                */
int get_transaction(transaction_t * transaction)
{
	uint8_t crc;
	int result;
	
	if (transaction == NULL) return E_SYS;
	
	while (writeptr < TRANSACTION_SIZE); 		// transaction still in progress
	
	memcpy(transaction, &serial_buffer[1], sizeof(transaction_t));
	
#ifdef CRC_CHECK	
	crc = crc8(serial_buffer, TRANSACTION_SIZE-1);
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
	uint8_t serialized[TRANSACTION_SIZE];
	int i;
	
	if (transaction == NULL) return E_SYS;
	
	serialized[0] = SOF;	// start frame marker
	memcpy(&serialized[1], transaction, sizeof(transaction_t));		// transaction contents
	serialized[TRANSACTION_SIZE-1] = crc8(serialized, TRANSACTION_SIZE-1);	// crc
	
	for (i=0; i<TRANSACTION_SIZE; i++) {
		while ( !(UCSR0A & (1<<UDRE0)) ); /* block till we have space */
		SERIAL_IF0_DATA = serialized[i]; /* send */
	}
	
	return E_OK;
}

