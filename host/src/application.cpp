#include <iostream>
#include "application.h"
#include "model/model.h"

using namespace std;

application::application()
{
	appModel = new model();
}

application::~application()
{
	delete appModel;
}

int application::start()
{
	int rval;
	
	cout << "Start application" << endl;
	
	rval = appModel->start();
	if (rval < 0) {
		cout << "Error initializing the model: Error #" << rval;
	} else {
		cout << "Everything is fine !";
	}
	
	cout << endl;
	
	return rval;
}
