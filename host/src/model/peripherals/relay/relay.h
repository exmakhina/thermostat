#ifndef __RELAY_H__
#define __RELAY_H__

#include "../peripherals.h"

class Relay : public Peripherals 
{
public:
	Relay();
	~Relay();
	
	int init();
	int close();
};

#endif /* __RELAY_H__ */
