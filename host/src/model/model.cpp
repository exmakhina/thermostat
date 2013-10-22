#include <vector>
#include <iostream>
#include "model.h"
#include "peripherals/serial_if/serialif.h"
#include "../controller/controller.h"

using namespace std;

Model::Model():
	ctrl(NULL)
{
	// Instantiate the peripherals
	SerialIF * tempSensorChain = new SerialIF();
	
	// Create the peripherals list
	peripheralsList.clear();
	peripheralsList.push_back(tempSensorChain);
}

Model::~Model()
{
	vector<Peripherals *>::const_iterator i;
	
	cout << "destroying all peripherals in the model..." << endl;
	
	for (i = peripheralsList.begin(); i != peripheralsList.end(); ++i) 
	{
		delete *i;
	}
	peripheralsList.clear();
}

int Model::start()
{
	int res;
	vector<Peripherals *>::const_iterator i;
	
	cout << "Starting Model" << endl;
	
	for (i = peripheralsList.begin(); i != peripheralsList.end(); ++i) 
	{
		res = (*i)->init();
		if (res < 0) {
			cout << "Error initializing peripheral" << endl;
			return res;
		}
	}
	
	return res; 
}

int Model::registerCtrl(Controller * controller)
{
	int rval = 0;
	
	if (controller != NULL) {
		ctrl = controller;
	} else {
		rval = -1;
	}
	
	return rval;
}
