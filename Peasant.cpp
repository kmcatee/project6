#include "Peasant.h"
#include "Structure.h"
#include "Model.h"
#include "Utility.h"
#include <iostream>
#define NDEBUG
#include <cassert>

using std::string;
using std::cout; using std::endl;
using std::shared_ptr;

const double food_capacity_c = 35.0;
const char* const stop_work_msg_c = ": I'm stopping work";

Peasant::Peasant(const string& name_, Point location_) :
	Agent(name_, location_), source(nullptr),
	destination(nullptr), food_carried(0.0), 
	work_state(Work_phase_e::Idle)
{}

void Peasant::update(){
	Agent::update();
	if (!is_alive() || work_state == Work_phase_e::Idle){
		return;
	}
	switch(work_state){
		case Work_phase_e::Inbound:
			if (!is_moving() && 
				get_location() == source->get_location()){
				work_state = Work_phase_e::Collecting;
			}
			break;
		case Work_phase_e::Collecting:
		{
			double received = source->withdraw(food_capacity_c - food_carried);
			food_carried += received;
			if (received > 0){
				cout << get_name() << ": Collected " << received << endl;
				work_state = Work_phase_e::Outbound;
				Agent::move_to(destination->get_location());
			}
			else {
				cout << get_name() << ": Waiting " << endl;
			}
			Model::get_instance().notify_amount(get_name(), food_carried);
			break;
		}
		case Work_phase_e::Outbound:
			if (!is_moving() &&
				get_location() == destination->get_location()){
				work_state = Work_phase_e::Depositing;
			}
			break;
		case Work_phase_e::Depositing:
			destination->deposit(food_carried);
			cout << get_name() << ": Deposited " << food_carried << endl;
			food_carried = 0.0;
			Agent::move_to(source->get_location());
			work_state = Work_phase_e::Inbound;
			Model::get_instance().notify_amount(get_name(), food_carried);
			break;
		default:
			assert(0);
			break;
	}
}

void Peasant::move_to(Point dest){
	if (work_state != Work_phase_e::Idle){
		cout << get_name() << stop_work_msg_c << endl;
		work_state = Work_phase_e::Idle;
		source = nullptr;
		destination = nullptr;
	}
	Agent::move_to(dest);
 }

void Peasant::stop(){
	Agent::stop();
	if (work_state != Work_phase_e::Idle){
		cout << get_name() << stop_work_msg_c << endl;
		work_state = Work_phase_e::Idle;
		source = nullptr;
		destination = nullptr;
	}
}

void Peasant::start_working(shared_ptr<Structure> source_, shared_ptr<Structure> destination_){
	Agent::stop();
	// set not_working state
	work_state = Work_phase_e::Idle;
	source = nullptr;
	destination = nullptr;
	if (source_ == destination_)
		throw Error {get_name() + ": I can't move food to and from the same place!"};
	source = source_;
	destination = destination_;
	if (food_carried == 0.0){
		if (get_location() == source->get_location()){
			work_state = Work_phase_e::Collecting;
		}
		else {
			Agent::move_to(source->get_location());
			work_state = Work_phase_e::Inbound;
		}
	}
	else {
		if (get_location() == destination->get_location()){
			work_state = Work_phase_e::Depositing;
		}
		else {
			Agent::move_to(destination->get_location());
			work_state = Work_phase_e::Outbound;
		}
	}
}

void Peasant::describe() const{
	cout << "Peasant ";
	Agent::describe();
	cout << "   Carrying " << food_carried << endl;
	switch(work_state){
		case Work_phase_e::Outbound:
			cout << "   Outbound to destination " 
			 	 << destination->get_name() << endl;
			break;
		case Work_phase_e::Inbound:
			cout << "   Inbound to source " 
			 	 << source->get_name() << endl;
			break;
		case Work_phase_e::Collecting:
			cout << "   Collecting at source " 
			 	 << source->get_name() << endl;
			break;
		case Work_phase_e::Depositing:
			cout << "   Depositing at destination " 
			 	 << destination->get_name() << endl;
			break;
		case Work_phase_e::Idle:
			// print nothing
			break;
		default:
			assert(0);

	}
}

void Peasant::broadcast_current_state(){
	Agent::broadcast_current_state();
	Model::get_instance().notify_amount(get_name(), food_carried);
}

