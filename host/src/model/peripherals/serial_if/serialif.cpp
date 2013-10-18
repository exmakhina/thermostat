#include <iostream>
#include <unistd.h>
#include <thread>
#include "../../../msgqueue.h"
#include "serialif.h"	// cpp header file
#include "serial_if.h"	// C interface file
#include <errno.h>   /* Error number definitions */

using namespace std;

SerialIF::SerialIF():
	fd(-1)
{
	eventThread = thread(eventLoop, this);
}

SerialIF::~SerialIF()
{
	cout << "Destroying serial interface" << endl;
	close();
	eventThread.join();
}

int SerialIF::init()
{
	int res;
	
	if (fd < 0) {
		cout << "Starting Serial controller" << endl;
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

void SerialIF::eventLoopRuntime()
{
	transaction_t message;
	
	while(1) {
		cout << "Wait for event..." << endl;
		msq.receive(message);
		cout << "Got a message !!" << endl;
	}
}

void * SerialIF::eventLoop(void * instance)
{
	cout << "starting eventLoop" << endl;
	
	static_cast<SerialIF*>(instance)->eventLoopRuntime();
	
	cout << "event loop finished" << endl;
	return NULL;
}

int SerialIF::getTemperature(float &temp, int addr)
{

}
