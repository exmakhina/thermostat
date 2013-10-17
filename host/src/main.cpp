#include <iostream>
#include "application.h"

void main()
{
	application *app = new application();
	
	cout << "Starting App..." << endl;
	app->start();
	
	cout << "Exiting" << endl;
}
