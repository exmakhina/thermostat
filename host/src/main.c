#include <stdio.h>   /* Standard input/output definitions */
#include "model/peripherals/serial_if/serial_if.h"

void main()
{
	int tty_fd = -1;
	
	printf("Testing thermostat....\n");
	
	tty_fd = serial_init();
	printf("Serial port open with id=%d\n", tty_fd);
	serial_close(tty_fd);
	
	printf("Serial port closed - Done.\n");
}
