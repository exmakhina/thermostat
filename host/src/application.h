#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "model/model.h"

class application 
{
public:
	application();
	~application();
	
	int start();
	
private:
	model *appModel;
};

#endif /* __APPLICATION_H__ */
