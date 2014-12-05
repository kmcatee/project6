#include "ETA_View.h"
#include <iostream>
#include <string>

using std::cout; using std::endl;
using std::string;

void ETA_View::draw(){
	cout << "Turns needed to reach destination" << ":" << endl;
	if(no_objects())
		cout << "No objects are currently moving" << endl;
	else
		List_View::draw();
}

void ETA_View::update_eta(const std::string& name, int turns){
	update_object(name, turns);
}

void ETA_View::update_stop_moving(const std::string& name){
	update_remove(name);
}