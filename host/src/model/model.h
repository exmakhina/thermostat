#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "peripherals/peripherals.h"

class Model
{
public:
	Model();	//\!TODO Add a (controller * ctrl) parameter
	~Model();
	
	int start();
	
private:
	std::vector<Peripherals *> peripheralsList;
};
#endif /* __MODEL_H__ */
