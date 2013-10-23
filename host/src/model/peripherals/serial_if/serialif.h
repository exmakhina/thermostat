#ifndef __SERIALIF_H__
#define __SERIALIF_H__

#include "../peripherals.h"
#include <thread>
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
#define 	SIG_READY	100
#define		SIG_KILL	101
	std::thread eventThread;
	static void *eventLoop(void *);
	void eventLoopRuntime();
	
	/* Message queues */
	MsgQueue<transaction_t> tx_msq;
	MsgQueue<transaction_t> rx_msq;
};

#endif /* __SERIALIF_H__ */
