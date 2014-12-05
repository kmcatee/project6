#include "Archer.h"
#include "Model.h"
#include "Structure.h"
#include <iostream>
#include <memory>

using std::cout; using std::endl;
using std::string;
using std::shared_ptr;

const int default_archer_atk_str_c = 1;
const double default_archer_atk_range_c = 6.0;

Archer::Archer(const string& name_, Point location_) :
	Warrior(name_, location_, default_archer_atk_str_c, default_archer_atk_range_c)
{}

void Archer::update(){
	Warrior::update();
	if (!is_attacking()){
		shared_ptr<Agent> target = Model::get_instance().get_nearest_agent(shared_from_this());
		if ((target && !target_out_of_range(target)))
			set_target(target);
	}
}

void Archer::take_hit(int attack_strength, shared_ptr<Agent> attacker_ptr){
	Warrior::take_hit(attack_strength, attacker_ptr);
	if (is_alive()){
		shared_ptr<Structure> destination = Model::get_instance().get_nearest_structure(shared_from_this());
		if (destination){
			cout << get_name() << ": I'm going to run away to " << destination->get_name() << endl;
			move_to(destination->get_location());
		}
	}
}

void Archer::describe() const {
	cout << "Archer ";
	Warrior::describe();
}

void Archer::print_attack_msg() const {
	cout << "Twang!";
}
