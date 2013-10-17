#include <iostream>
#include "application.h"

using namespace std;

int main()
{
	application *app = new application();
	
	cout << "Starting App..." << endl;
	app->start();
	
	cout << "Exiting" << endl;
	
	return 0;
}
