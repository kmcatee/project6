#include "Farm.h"
#include "Model.h"
#include <iostream>

using std::string;
using std::cout; using std::endl;

const double initial_food_c = 50.0;
const double production_rate_c = 2.0;

Farm::Farm(const string& name_, Point location_) :
	Structure(name_, location_), food_amount(initial_food_c),
	production_rate(production_rate_c)
{}

double Farm::withdraw(double amount_to_get){
	if (amount_to_get > food_amount){
		amount_to_get = food_amount;
	}
	food_amount -= amount_to_get;
	Model::get_instance().notify_amount(get_name(), food_amount);
	return amount_to_get;
}

void Farm::update(){
	food_amount += production_rate;
	cout << "Farm " << get_name() << " now has " << food_amount << endl;
	Model::get_instance().notify_amount(get_name(), food_amount);
}

void Farm::describe() const{
	cout << "Farm ";
	Structure::describe();
	cout << "   Food available: " << food_amount << endl;
}

void Farm::broadcast_current_state(){
	Structure::broadcast_current_state();
	Model::get_instance().notify_amount(get_name(), food_amount);
}
