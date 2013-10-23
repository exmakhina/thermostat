#include <iostream>
#include "application.h"
#include "model/model.h"
#include "controller/controller.h"
#include "views/view.h"

using namespace std;

Application::Application()
{
	model = new Model();
	ctrl = new Controller(model);
	
	model->registerCtrl(ctrl);
}

Application::~Application()
{
	delete ctrl;
	delete model;
}

int Application::start()
{
	int rval;
	float temp;
	
	cout << "Start application" << endl;
	
	rval = ctrl->start();		// Controller starts its associated Model and Views
	if (rval < 0) {
		cout << "Error initializing the Application: Error #" << rval;
	}
	
	model->readTemperature(temp, 1);
	
	cout << endl << "Temperature is " << temp << endl;;
	
	return rval;
}
