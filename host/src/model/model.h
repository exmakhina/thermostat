#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "peripherals/peripherals.h"

class model
{
public:
	model();	//\!TODO Add a (controller * ctrl) parameter
	~model();
	
	int start();
	
private:
	std::vector<peripherals *> peripheralsList;
};
#endif /* __MODEL_H__ */
