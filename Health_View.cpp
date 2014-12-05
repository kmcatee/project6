#include "Health_View.h"
#include <iostream>
#include <string>

using std::cout; using std::endl;
using std::string;

void Health_View::draw(){
	cout << "Current " << "Health" << ":" << endl;
	List_View::draw();
}

void Health_View::update_health(const string& name, int health){
	update_object(name, health);
}
