#include <vector>
#include <iostream>
#include "model.h"
#include "peripherals/serial_if/serialif.h"

using namespace std;

model::model()
{
	serialif * tempSensorChain = new serialif();
	
	peripheralsList.clear();
	peripheralsList.push_back(tempSensorChain);
}

model::~model()
{
	peripheralsList.clear();
}

int model::start()
{
	int res;
	vector<peripherals *>::const_iterator i;
	
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
