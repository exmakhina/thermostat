#ifndef __SERIALIF_H__
#define __SERIALIF_H__

#include "../peripherals.h"

class SerialIF : public Peripherals 
{
public:
	SerialIF();
	~SerialIF();
	
	int init();
	int close();
	
private:
	int fd;
};

#endif /* __SERIALIF_H__ */
