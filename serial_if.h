#ifndef _SERIAL_IF_H_
#define _SERIAL_IF_H_

#include <stdio.h>
#include "libarduino.h"

#define SERIAL_IF_BAUD_RATE		9600 /* default is 9600 */ 
#define SERIAL_IF_BAUD_SELECT		(F_CPU / (SERIAL_IF_BAUD_RATE * 16L) - 1)

#if	defined(__AVR_ATmega168__) || \
 	defined(__AVR_ATmega328P__)

#define SERIAL_IF0_DATA		UDR0
#define SERIAL_IF0_ISR_VECT 	USART_RX_vect

#else
// unsupported type
#error "Processor type not supported in serial_if.h !"
#endif

/*****************************************************
 transaction_t: structure defining the transaction
 field. It gets serialized/desrialized to/from
 the following bitfield:
 
 |31|..|28|27|..|24|23|....................|7|.......|0|
 |   CMD  |  ADDR  |       VALUE           |    CRC8   |
 
 *****************************************************/
typedef struct transaction_s {
	uint8_t 	cmd;
	uint8_t 	addr;
	int		value;
	uint8_t 	crc;
} transaction_t;

#define E_OK		0
#define E_SYS		-1
#define E_NOT_READY	-2
#define E_CRC		-3

#define CMD_MASK	0xf0
#define ADDR_MASK	0x0f

#define RESP_ACK	0x00
#define RESP_NAK	0x01
#define RESP_ERROR	0xff

#define CMD_GET_TEMP	0x01
#define CMD_SET_RELAY	0x02

void serial_init(void);
int get_transaction(transaction_t * transaction);
int send_transaction(transaction_t * transaction);

#endif	// _SERIAL_IF_H_
