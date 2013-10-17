#include <iostream>
#include "application.h"
#include "model/model.h"

using namespace std;

Application::Application()
{
	model = new Model();
}

Application::~Application()
{
	delete model;
}

int Application::start()
{
	int rval;
	
	cout << "Start application" << endl;
	
	rval = model->start();
	if (rval < 0) {
		cout << "Error initializing the model: Error #" << rval;
	} else {
		cout << "Everything is fine !";
	}
	
	cout << endl;
	
	return rval;
}
