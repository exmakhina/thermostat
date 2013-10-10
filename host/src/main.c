#include <stdio.h>   /* Standard input/output definitions */
#include "serial_if.h"

void main()
{
	int tty_fd;
	
	printf("Testing thermostat....\n");
	
	tty_fd = serial_init();
	serial_close(tty_fd);
	
	printf("Done.\n");
}
