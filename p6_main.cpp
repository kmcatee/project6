/*
Main module. Your project must use this file as-is
*/

#include <iostream>
#include "Controller.h"

using namespace std;

// The main function simply creates the Controller object and tells it to run. 
int main ()
{		
	// Set output to show two decimal places
//	cout << fixed << setprecision(2) << endl;
	cout.setf(ios::fixed, ios::floatfield);
	cout.precision(2);

	// create the Controller object and go
	Controller controller;

	controller.run();
}

