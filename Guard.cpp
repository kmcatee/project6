#include "Guard.h"
#include "Model.h"
#include <iostream>
#include <memory>

using std::cout; using std::endl;
using std::string;
using std::shared_ptr;

const int default_guard_atk_str_c = 2;
const double default_guard_atk_range_c = 2.0;

Guard::Guard(const string& name_, Point location_) :
	Warrior(name_, location_, default_guard_atk_str_c, default_guard_atk_range_c)
{}

void Guard::start_protecting(std::shared_ptr<Agent> charge_ptr){
	if (charge_ptr.get() == this)	
		throw Error {get_name() + ": I cannot guard myself!"};
	if (!charge_ptr->is_alive())
		throw Error {get_name() + ": Target is not alive!"};
	set_charge(target_ptr);
}