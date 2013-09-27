#include <avr/version.h>
#if __AVR_LIBC_VERSION__ < 10606UL
#error "please update to avrlibc 1.6.6 or newer, not tested with older versions"
#endif


#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <string.h>
#include <stdint.h>

#include <libarduino.h>
#include "serial_if.h"

#define MAXSENSORS 	5
#define MAXRELAYS	4

#define RELAY_1		8
#define RELAY_2		9
#define RELAY_3		10
#define RELAY_4		11

uint8_t gSensorIDs[MAXSENSORS][OW_ROMCODE_SIZE];

void init_relays(void)
{
	pinMode(RELAY_1, OUTPUT);
	pinMode(RELAY_2, OUTPUT);
	pinMode(RELAY_3, OUTPUT);
	pinMode(RELAY_4, OUTPUT);
	
	// relay command is active low
	digitalWrite(RELAY_1, HIGH);
	digitalWrite(RELAY_2, HIGH);
	digitalWrite(RELAY_3, HIGH);
	digitalWrite(RELAY_4, HIGH);
}

static uint8_t search_sensors(void)
{
	uint8_t i;
	uint8_t id[OW_ROMCODE_SIZE];
	uint8_t diff, nSensors;
	
	printf(  "Scanning Bus for DS18X20\r\n");
	
	ow_reset();

	nSensors = 0;
	
	diff = OW_SEARCH_FIRST;
	while ( diff != OW_LAST_DEVICE && nSensors < MAXSENSORS ) {
		DS18X20_find_sensor( &diff, &id[0] );
		
		if( diff == OW_PRESENCE_ERR ) {
			printf( "No Sensor found\r\n");
			break;
		}
		
		if( diff == OW_DATA_ERR ) {
			printf( "Bus Error\r\n");
			break;
		}
		
		for ( i=0; i < OW_ROMCODE_SIZE; i++ )
			gSensorIDs[nSensors][i] = id[i];
		
		nSensors++;
	}
	
	return nSensors;
}

#if DS18X20_EEPROMSUPPORT

static void th_tl_dump(uint8_t *sp)
{
	DS18X20_read_scratchpad( &gSensorIDs[0][0], sp, DS18X20_SP_SIZE );
	printf( "TH/TL in scratchpad of sensor 1 now : " );
	printf("%d",sp[DS18X20_TH_REG] );
	printf( " / " ); 
	printf("%d",sp[DS18X20_TL_REG] );
	printf( "\r\n"); 
}

static void eeprom_test(void)
{
	uint8_t sp[DS18X20_SP_SIZE], th, tl;
	
	printf(  "DS18x20 EEPROM support test for first sensor\r\n"); 
	// DS18X20_eeprom_to_scratchpad(&gSensorIDs[0][0]); // already done at power-on
	th_tl_dump( sp );
	th = sp[DS18X20_TH_REG];
	tl = sp[DS18X20_TL_REG];
	tl++;
	th++;
	DS18X20_write_scratchpad( &gSensorIDs[0][0], th, tl, DS18B20_12_BIT );
	printf( "TH+1 and TL+1 written to scratchpad\r\n");
	th_tl_dump( sp );
	DS18X20_scratchpad_to_eeprom( DS18X20_POWER_PARASITE, &gSensorIDs[0][0] );
	printf( "scratchpad copied to DS18x20 EEPROM\r\n");
	DS18X20_write_scratchpad( &gSensorIDs[0][0], 0, 0, DS18B20_12_BIT );
	printf( "TH and TL in scratchpad set to 0\r\n");
	th_tl_dump( sp );
	DS18X20_eeprom_to_scratchpad(&gSensorIDs[0][0]);
	printf( "DS18x20 EEPROM copied back to scratchpad\r\n");
	DS18X20_read_scratchpad( &gSensorIDs[0][0], sp, DS18X20_SP_SIZE );
	if ( ( th == sp[DS18X20_TH_REG] ) && ( tl == sp[DS18X20_TL_REG] ) ) {
		printf( "TH and TL verified\r\n");
	} else {
		printf( "verify failed\r\n");
	}
	th_tl_dump( sp );
}
#endif /* DS18X20_EEPROMSUPPORT */

/* State = 1 -> relay ON, state = 0 -> relay OFF*/
void switch_relay(uint8_t id, uint8_t state) {
	enum pinstate pin_state;
	int relay_pin;
	
	if (state == 0)
		pin_state = HIGH;
	else
		pin_state = LOW;
		
	switch (id) {
	case 1:
		relay_pin = RELAY_1;
	break;
	case 2:
		relay_pin = RELAY_2;
	break;
	case 3:
		relay_pin = RELAY_3;
	break;
	case 4:
		relay_pin = RELAY_4;
	break;
	}
	digitalWrite(relay_pin, pin_state);
}

int main( void )
{
	uint8_t nSensors, i;
	int16_t decicelsius;
	uint8_t error;
	transaction_t transaction;
	transaction_t response = {0};
	int rval;
	
	serial_init();
	
	init_relays();
	
#ifndef OW_ONE_BUS
	ow_set_bus(&PIND,&PORTD,&DDRD,PD6);
#endif
	
	sei();
	
	// initialize sensor (scan onewire bus + eeprom test)
	nSensors = search_sensors();
	eeprom_test();

	while (1) {
		memset(&response, 0, sizeof(transaction_t));	// reset response
		
		rval = receive_transaction(&transaction);
		if (rval == E_NOT_READY) continue;
		if (rval == E_SYS) return 1;
		if (rval == E_CRC) {
			response.cmd = RESP_NAK;
		} 
		else if (rval == E_OK) {
			switch(transaction.cmd) {
			case CMD_GET_TEMP:
				if (transaction.addr >= nSensors) {
					response.addr = transaction.addr;
					goto error;
				}
				if ( DS18X20_start_meas( DS18X20_POWER_PARASITE, NULL ) == DS18X20_OK) {
					_delay_ms( DS18B20_TCONV_12BIT );
					if ( DS18X20_read_decicelsius( &gSensorIDs[transaction.addr][0], &decicelsius ) == DS18X20_OK ) {
						response.cmd = RESP_ACK;
						response.addr = transaction.addr;
						response.value = decicelsius;
						break;
					} else goto error;
				} else goto error;
				break;
			case CMD_SET_RELAY:
				if (transaction.addr >= MAXRELAYS) {
					response.addr = transaction.addr;
					goto error;
				}
				switch_relay(transaction.addr, transaction.value);
				response.cmd = RESP_ACK;
				response.addr = transaction.addr;
				response.value = transaction.value;
				break;
			default:
error:
				response.cmd = RESP_ERROR;
			}
		}
		send_response(&response);
	}
}


