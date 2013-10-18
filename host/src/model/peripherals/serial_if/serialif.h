#ifndef __SERIALIF_H__
#define __SERIALIF_H__

#include "../peripherals.h"
#include <pthread.h>
#include "../../../msgqueue.h"
#include "serial_if.h"	// C interface file

class SerialIF : public Peripherals 
{
public:
	SerialIF();
	~SerialIF();
	
	int init();
	int close();
	
	int getTemperature(float &temp, int addr);
	
private:
	int fd;
	
	/* Communication thread stuff */
	pthread_t eventThread;
	static void *eventLoop(void *);
	void eventLoopRuntime();
	
	/* Message queue */
	MsgQueue<transaction_t> msq;
};

#endif /* __SERIALIF_H__ */
