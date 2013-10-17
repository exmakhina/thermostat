#ifndef __RELAY_H__
#define __RELAY_H__

#include "../peripherals.h"

class relay : public peripherals 
{
public:
	relay();
	~relay();
	
	int init();
	int close();
};

#endif /* __RELAY_H__ */
