#include "serialif.h"	// cpp header file
#include "serial_if.h"	// C interface file
#include <errno.h>   /* Error number definitions */

serialif::serialif():
	fd(-1)
{
}

serialif::~serialif()
{
	close();
}

int serialif::init()
{
	fd = serial_init();
	if (fd < 0)
		return fd;
	else
		return 0;
}

int serialif::close()
{
	serial_close(fd);
	return 0;
}


