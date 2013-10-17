#ifndef _SERIAL_IF_H_
#define _SERIAL_IF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define SERIAL_IF_PORT			"/dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_741333535373519132D0-if00"
#define SERIAL_IF_BAUD_RATE		B9600 /* default is 9600 */ 

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

int serial_init(void);
void serial_close(int tty_fd);
int receive_transaction(int tty_fd, transaction_t * transaction);
int send_response(int tty_fd, transaction_t * transaction);

#ifdef __cplusplus
}
#endif


#endif	// _SERIAL_IF_H_
