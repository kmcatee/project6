#include "Agent.h"
#include "Model.h"
#include "Geometry.h"
#include "Utility.h"
#include <iostream>
#define NDEBUG
#include <cassert>
#include <cmath>

using std::string;
using std::cout; using std::endl;
using std::shared_ptr;

const int initial_health_c = 5;
const int default_speed_c = 5;

Agent::Agent(const string& name_, Point location_) :
	Sim_object(name_), movement(location_, default_speed_c), 
	health(initial_health_c), alive_state(Alive_e::Alive)
{}

void Agent::move_to(Point destination_){
	movement.start_moving(destination_);
	if (movement.is_currently_moving()){
		cout << get_name() << ": I'm on the way" << endl;
		notify_moving_helper();
	}
	else {
		cout << get_name() << ": I'm already there" << endl;
	}
}

void Agent::stop(){
	if (movement.is_currently_moving()){
		movement.stop_moving();
		cout << get_name() << ": I'm stopped" << endl;
	}
}

void Agent::take_hit(int attack_strength, shared_ptr<Agent> attacker_ptr){
	lose_health(attack_strength);
}

void Agent::update(){
	switch(alive_state){
		case Alive_e::Alive:
			if (is_moving()){
				if (movement.update_location()){
					cout << get_name() << ": I'm there!"<< endl;
					Model::get_instance().notify_stop_moving(get_name());
				}
				else{
					cout << get_name() << ": step..." << endl;
					notify_moving_helper();
				}
				Model::get_instance().notify_location(get_name(), movement.get_current_location());
			}
			break;
		case Alive_e::Dead:
			// do nothing
			break;
		default:
			assert(0);
	}
}

void Agent::describe() const{
	cout << get_name() << " at " << movement.get_current_location() << endl;
	switch(alive_state){
		case Alive_e::Alive:
			cout << "   Health is " << health << endl;
			if (movement.is_currently_moving()){
				cout << "   Moving at speed " << movement.get_current_speed() 
					 << " to " << movement.get_current_destination() << endl;
			}
			else {
				cout << "   Stopped" << endl;
			}
			break;
		case Alive_e::Dead:
			cout << "   Is dead" << endl;
			break;
		default:
			assert(0);
	}
}

void Agent::broadcast_current_state(){
	Model::get_instance().notify_location(get_name(), movement.get_current_location());
	Model::get_instance().notify_health(get_name(), health);
	if(is_moving()){
		notify_moving_helper();
	}
}

void Agent::start_working(shared_ptr<Structure>, std::shared_ptr<Structure>){
	throw Error {get_name() + ": Sorry, I can't work!"};
}

void Agent::start_attacking(shared_ptr<Agent>){
	throw Error {get_name() + ": Sorry, I can't attack!"};
}

void Agent::start_protecting(shared_ptr<Agent>){
	throw Error {get_name() +": Sorry, I can't protect!"};
}

void Agent::lose_health(int attack_strength){
	health -= attack_strength;
	if (health <= 0){
		alive_state = Alive_e::Dead;
		Model::get_instance().notify_gone(get_name());
		movement.stop_moving();
		cout << get_name() << ": Arrggh!" << endl;
		Model::get_instance().remove_agent(shared_from_this());
	}
	else {
		cout << get_name() << ": Ouch!" << endl;
		Model::get_instance().notify_health(get_name(), health);
	}	
}

void Agent::notify_moving_helper(){
	double distance = cartesian_distance(movement.get_current_location(), movement.get_current_destination());
	int time_to_dest = ceil(distance / movement.get_current_speed());
	Model::get_instance().notify_eta(get_name(), time_to_dest);
}