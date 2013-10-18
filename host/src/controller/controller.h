#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <list>
#include "../views/view.h"

class Model;

class Controller 
{
public:
	Controller(Model *);
	~Controller();
	
	int start();
	int registerView(View *);
	
private:
	Model *model;
	std::list<View *> viewList;
};

#endif /* __CONTROLLER_H__ */
