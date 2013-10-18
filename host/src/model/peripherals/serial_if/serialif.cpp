#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include "serialif.h"	// cpp header file
#include "serial_if.h"	// C interface file
#include <errno.h>   /* Error number definitions */

using namespace std;

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
	int res;
	
	if (fd < 0) {
		cout << "Starting Serial controller" << endl;
		fd = serial_init();
		if (fd < 0)	return fd;
		
		res = pthread_create(&eventThread, NULL, eventLoop, this);
		if (res != 0) {
			cout << "Error creating the serial IF event thread" << endl;
			serial_close(fd);
			fd = -1;
			return -1;
		}
	}
	
	return 0;	// in case where no initialization is necesary or sucess
}

int SerialIF::close()
{
	pthread_join(eventThread, NULL);
	
	serial_close(fd);
	fd = -1;
	
	return 0;
}

void SerialIF::eventLoopRuntime()
{
	while(1) {
		cout << "eventLoopRuntime is alive" << endl;
		usleep(1000000);
	}
	
	pthread_exit(NULL);
}

void * SerialIF::eventLoop(void * instance)
{
	static_cast<SerialIF*>(instance)->eventLoopRuntime();
	return NULL;
}
