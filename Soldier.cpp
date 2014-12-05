#include "Soldier.h"
#include <iostream>

using std::cout; using std::endl;
using std::shared_ptr;
using std::string;

const int default_soldier_atk_str_c = 2;
const double default_soldier_atk_range_c = 2.0;

Soldier::Soldier(const string& name_, Point location_) :
	Warrior(name_, location_, default_soldier_atk_str_c, default_soldier_atk_range_c)
{}

void Soldier::take_hit(int attack_strength, shared_ptr<Agent> attacker_ptr){
	Warrior::take_hit(attack_strength, attacker_ptr);
	if (!is_attacking() && is_alive() && attacker_ptr->is_alive()){
		set_target(attacker_ptr);
	}
}

void Soldier::describe() const{
	cout << "Soldier ";
	Warrior::describe();
}

void Soldier::print_attack_msg() const {
	cout << "Clang!";
}
