#include <iostream>
#include "application.h"

using namespace std;

int main()
{
	Application *app = new Application();
	
	cout << "Starting App..." << endl;
	app->start();
	
	cout << "Exiting" << endl;
	
	delete app;
	
	return 0;
}
