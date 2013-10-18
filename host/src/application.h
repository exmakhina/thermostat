#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "model/model.h"
#include "controller/controller.h"
#include "views/view.h"

class Application 
{
public:
	Application();
	~Application();
	
	int start();
	
private:
	Model *model;
	Controller *ctrl;
};

#endif /* __APPLICATION_H__ */
