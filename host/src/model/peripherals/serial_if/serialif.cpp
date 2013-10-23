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
}

SerialIF::~SerialIF()
{
	transaction_t message;
	
	cout << "Destroying serial interface" << endl;
	close();
	message.cmd = SIG_KILL;
	tx_msq.send(message);
	eventThread.join();
}

int SerialIF::init()
{
	int res;
	transaction_t message;
	
	if (fd < 0) {
		cout << "Starting Serial controller" << endl;
		fd = serial_init();
		if (fd < 0)	return fd;
		eventThread = thread(eventLoop, this);
		cout << "Wait for the READY signal." << endl;
		rx_msq.receive(message);
		if (message.cmd != SIG_READY) {
			cout << "Error starting the serial IF event loop thread." << endl;
			return -1; 
		}
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
	transaction_t response;
	bool stop = false;
	
	message.cmd = SIG_READY;
	rx_msq.send(message);
	
	while(!stop) {
		cout << "Wait for event..." << endl;
		tx_msq.receive(message);
		switch (message.cmd) {
		case CMD_GET_TEMP:
			cout << "Start reading temperature from sensor #" << message.addr << endl;
			response.cmd = RESP_ACK;
			response.value = 250;
			response.addr = 1;
			rx_msq.send(response);
		break;
		
		case CMD_SET_RELAY:
			cout << "Switching relay #" << message.addr << endl;
		break;
		
		case SIG_KILL:
			cout << "Receive a KILL signal." << endl;
			stop = true;
		break;
		
		default:
			cout << "Got an unknown message (" <<message.cmd << ")" << endl;
		}
	}
}

void * SerialIF::eventLoop(void * instance)
{
	static_cast<SerialIF*>(instance)->eventLoopRuntime();
	
	return NULL;
}

int SerialIF::getTemperature(float &temp, int addr)
{
	transaction_t message;
	int rval = -1;
	
	cout << "Sending request for reading sensor #" <<addr <<endl;
	message.cmd = CMD_GET_TEMP;
	message.addr = addr;
	message.value = 0;
	tx_msq.send(message);
	
	cout << "Wait for response..." <<endl;
	rx_msq.receive(message);
	if (message.cmd != RESP_ACK) {
		cout << "Response error !" <<endl;
		return rval;
	}
	cout << "Got acknoledgement" << endl;
	
	temp = message.value / 10.0;
	rval = 0;
	
	return rval;
}
