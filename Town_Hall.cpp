#include "Town_Hall.h"
#include "Model.h"
#include <iostream>

using std::string;
using std::cout; using std::endl;

// amount of food given as taxes during a withdrawal
// This must be an amount between 0.0 and 1.0
const double tax_percentage_c = 0.1;

Town_Hall::Town_Hall(const string& name_, Point location_) :
	Structure(name_, location_), food_amount(0)
{}

void Town_Hall::deposit(double deposit_amount){
	food_amount += deposit_amount;
	Model::get_instance().notify_amount(get_name(), food_amount);
}

double Town_Hall::withdraw(double amount_to_obtain){
	// amount of food available reduced by tax
	double amount_available = food_amount - (food_amount * tax_percentage_c);
	if (amount_available < 1.0){
		amount_available = 0.0;
	}
	if (amount_to_obtain > amount_available){
		amount_to_obtain = amount_available;
	}
	food_amount -= amount_to_obtain;
	Model::get_instance().notify_amount(get_name(), food_amount);
	return amount_to_obtain;
}

void Town_Hall::describe() const{
	cout << "Town_Hall ";
	Structure::describe();
	cout << "   Contains " << food_amount << endl;
}

void Town_Hall::broadcast_current_state(){
	Structure::broadcast_current_state();
	Model::get_instance().notify_amount(get_name(), food_amount);
}
