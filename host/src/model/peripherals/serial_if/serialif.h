#ifndef __SERIALIF_H__
#define __SERIALIF_H__

#include "../peripherals.h"
#include <pthread.h>

class SerialIF : public Peripherals 
{
public:
	SerialIF();
	~SerialIF();
	
	int init();
	int close();
	
private:
	int fd;
	
	pthread_t eventThread;
	static void *eventLoop(void *);
	void eventLoopRuntime();
};

#endif /* __SERIALIF_H__ */
