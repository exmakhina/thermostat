#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "model/model.h"

class Application 
{
public:
	Application();
	~Application();
	
	int start();
	
private:
	Model *model;
};

#endif /* __APPLICATION_H__ */
