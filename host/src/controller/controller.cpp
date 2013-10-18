#include <list>
#include <cstddef>
#include <iostream>
#include "controller.h"
#include "../model/model.h"
#include "../views/view.h"

using namespace std;

Controller::Controller(Model * mdl):
	model(mdl)
{
	viewList.clear();
}

Controller::~Controller()
{
	viewList.clear();
	model = NULL;
}

int Controller::start()
{
	int rval = 0;
	list<View *>::const_iterator i;
	
	cout << "Starting Controller" << endl;
	if (model != NULL) {
		rval = model->start();
	}
	
	for (i = viewList.begin(); i != viewList.end(); ++i) {
		cout << "Starting views :" << endl;
		(*i)->start();
	}
	
	return rval;
}

int Controller::registerView(View *view)
{
	int rval = 0;
	
	if (view != NULL) {
		viewList.push_back(view);
	} else {
		rval = -1;
	}
	
	return rval;
}
