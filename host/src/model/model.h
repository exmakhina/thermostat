#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <list>
#include "peripherals/peripherals.h"

class Controller;

class Model
{
public:
	Model();	
	~Model();
	
	int start();
	int registerCtrl(Controller * controller);
	
private:
	std::vector<Peripherals *> peripheralsList;
	Controller * ctrl;
};

#endif /* __MODEL_H__ */
