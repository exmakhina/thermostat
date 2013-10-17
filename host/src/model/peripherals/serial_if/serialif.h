#ifndef __SERIALIF_H__
#define __SERIALIF_H__

#include "../peripherals.h"

class serialif : public peripherals 
{
public:
	serialif();
	~serialif();
	
	int init();
	int close();
	
private:
	int fd;
};

#endif /* __SERIALIF_H__ */
