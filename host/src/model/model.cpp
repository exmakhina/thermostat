#include <vector>
#include <iostream>
#include "model.h"
#include "peripherals/serial_if/serialif.h"

using namespace std;

Model::Model()
{
	SerialIF * tempSensorChain = new SerialIF();
	
	peripheralsList.clear();
	peripheralsList.push_back(tempSensorChain);
}

Model::~Model()
{
	peripheralsList.clear();
}

int Model::start()
{
	int res;
	vector<Peripherals *>::const_iterator i;
	
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
