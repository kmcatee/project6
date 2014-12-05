#include "Amount_View.h"
#include <iostream>

using std::cout; using std::endl;
using std::string;

void Amount_View::draw(){
	cout << "Current " << "Amounts" << ":" << endl;
	List_View::draw();
}

void Amount_View::update_amount(const string& name, double amount){
	update_object(name, amount);
}
