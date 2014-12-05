#include "Warrior.h"
#include "Model.h"
#include "Utility.h"
#include <iostream>
#define NDEBUG
#include <cassert>

using std::string;
using std::cout; using std::endl;
using std::weak_ptr; using std::shared_ptr;



Warrior::Warrior(const string& name_, Point location_, int attack_str_, double attack_range_)
	:Agent(name_, location_), attack_str(attack_str_), attack_range(attack_range_),
	attacking_state(false)
{}

void Warrior::update(){
	Agent::update();
	if (!is_alive() || !attacking_state)
		return;
	shared_ptr<Agent> target_ptr = current_target.lock();
	// test if target has been deleted or is dead
	if (!target_ptr || !target_ptr->is_alive()){
		cout << get_name() << ": Target is dead" << endl;
		current_target.reset();
		attacking_state = false;
	}
	// else: target_ptr is valid
	else if (target_out_of_range(target_ptr)){
		cout << get_name() << ": Target is now out of range" << endl;
		current_target.reset();
		attacking_state = false;
	}
	else { // attack target!
		cout << get_name() << ": ";
		print_attack_msg();
		cout << endl;
		target_ptr->take_hit(attack_str, shared_from_this());
		if (!target_ptr->is_alive()){ //just killed target
			cout << get_name() << ": I triumph!" << endl;
			current_target.reset();
			attacking_state = false;
		}
	}
}

void Warrior::start_attacking(shared_ptr<Agent> target_ptr){
	if (target_ptr.get() == this)	
		throw Error {get_name() + ": I cannot attack myself!"};
	if (!target_ptr->is_alive())
		throw Error {get_name() + ": Target is not alive!"};
	if (target_out_of_range(target_ptr))
		throw Error {get_name() + ": Target is out of range!"};
	set_target(target_ptr);
}

void Warrior::take_hit(int attack_strength, shared_ptr<Agent> attacker_ptr){
	lose_health(attack_strength);
	if (!current_target.expired()){ 
		if (!is_alive()){
			current_target.reset();
			attacking_state = false;
		}		
	}
}

void Warrior::stop(){
	cout << get_name() << ": Don't bother me" << endl;
}

void Warrior::describe() const{
	Agent::describe();
	if (!attacking_state){
		cout << "   Not attacking" << endl;
		return;
	}
	shared_ptr<Agent> target_ptr = current_target.lock();
	if (target_ptr){
		cout << "   Attacking " 
			 << target_ptr->get_name() << endl;	
	}
	else {
		cout << "   Attacking dead target" << endl;
	}
}

void Warrior::set_target(shared_ptr<Agent> new_target){
	current_target = new_target;
	attacking_state = true;
	cout << get_name() << ": I'm attacking!" << endl;
}

bool Warrior::target_out_of_range(shared_ptr<Agent> target_ptr){
	assert (target_ptr);
	return cartesian_distance(get_location(), target_ptr->get_location())
		> attack_range; 
}
