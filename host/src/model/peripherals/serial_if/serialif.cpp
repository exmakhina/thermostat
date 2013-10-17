#include "serialif.h"	// cpp header file
#include "serial_if.h"	// C interface file
#include <errno.h>   /* Error number definitions */

SerialIF::SerialIF():
	fd(-1)
{
}

SerialIF::~SerialIF()
{
	close();
}

int SerialIF::init()
{
	if (fd < 0) {
		fd = serial_init();
		if (fd < 0)	return fd;
	}
	
	return 0;	// in case where no initialization is necesary or sucess
}

int SerialIF::close()
{
	serial_close(fd);
	fd = -1;
	
	return 0;
}


