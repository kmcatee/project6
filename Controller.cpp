#include "Controller.h"
#include "Model.h"
#include "Map_View.h"
#include "Local_View.h"
#include "Health_View.h"
#include "Amount_View.h"
#include "ETA_View.h"
#include "Agent.h"
#include "Structure.h"
#include "Agent_factory.h"
#include "Structure_factory.h"
#include "Utility.h"
#include <iostream>
#include <utility>
#include <functional>
#include <exception>
#include <cctype>
#include <algorithm>
#define NDEBUG
#include <cassert>

using std::string;
using std::cin;
using std::cout; using std::endl;
using std::shared_ptr;
using std::make_shared;

using std::make_pair;
using std::mem_fn;
using std::exception;
using std::isalnum;
using std::for_each;
using std::find;

using View_cmd_itr_t = std::map<string, void(Controller::*)(shared_ptr<View>)>::iterator;
using Program_cmd_itr_t = std::map<string, void(Controller::*)()>::iterator;
using Agent_cmd_itr_t = std::map<string, void(Controller::*)(shared_ptr<Agent>)>::iterator;

const size_t minimum_name_length_c = 2;
const string quit_command_string_c = "quit";
const char* const invalid_name_msg_c = "Invalid name for new object!";
const char* const unrecognized_command_msg_c = "Unrecognized command!";
const char* const map_not_open_msg_c = "No map view is open!";
const string map_view_name_c = "map";
const string health_view_name_c = "health";
const string amount_view_name_C = "amounts";
const string eta_view_name_c = "eta";

// helper declarations
static void print_prompt();
static int get_int_input();
static double get_double_input();
static Point get_Point_input();
static bool name_is_invalid(const string& name);

Controller::Controller()
{
	program_commands.insert(make_pair("open", &Controller::open_command));
	program_commands.insert(make_pair("close", &Controller::close_command));

	program_commands.insert(make_pair("default", &Controller::default_command));
	program_commands.insert(make_pair("size", &Controller::size_command));
	program_commands.insert(make_pair("zoom", &Controller::zoom_command));
	program_commands.insert(make_pair("pan", &Controller::pan_command));
	program_commands.insert(make_pair("show", &Controller::show_command));

	program_commands.insert(make_pair("status", &Controller::status_command));
	program_commands.insert(make_pair("go", &Controller::go_command));
	program_commands.insert(make_pair("build", &Controller::build_command));
	program_commands.insert(make_pair("train", &Controller::train_command));

	agent_commands.insert(make_pair("move", &Controller::move_command));
	agent_commands.insert(make_pair("work", &Controller::work_command));
	agent_commands.insert(make_pair("attack", &Controller::attack_command));
	agent_commands.insert(make_pair("stop", &Controller::stop_command));
}

void Controller::run(){
	string command; 
	print_prompt();
	while (cin >> command && command != quit_command_string_c){
		try{
			if (Model::get_instance().is_agent_present(command)){
				shared_ptr<Agent> agent_ptr = Model::get_instance().get_agent_ptr(command);
				// any dead agent should be removed from Model immediately
				assert(agent_ptr->is_alive());
				cin >> command;
				Agent_cmd_itr_t a_cmd_itr = agent_commands.find(command);
				if (a_cmd_itr == agent_commands.end())
					throw Error {unrecognized_command_msg_c};
				(this->*agent_commands[command])(agent_ptr); 
			}
			else { // look command up in program commands.
				Program_cmd_itr_t p_cmd_itr = program_commands.find(command);
				if (p_cmd_itr == program_commands.end())
					throw Error {unrecognized_command_msg_c};
				(this->*(p_cmd_itr->second))();
			}
		}
		catch(exception& ex){
			cout << ex.what() << endl;
			while (cin.get() != '\n');
		}
		catch(...){
			cout << "Unknown exception caught!" << endl;
			break;
		}
		print_prompt();
	}
	cout << "Done" << endl;
}

// prints prompt for user to enter a command, uses Model's current time.
void print_prompt(){
	cout << "\nTime " << Model::get_instance().get_time() << ": Enter command: ";
}

void Controller::open_command(){
	string view_name;
	cin >> view_name;
	auto name_itr = views.find(view_name);
	if (name_itr != views.end())
		throw Error {"View of that name already open!"};
	// check the view type
	if (view_name == map_view_name_c){
		map_view = make_shared<Map_View>(Map_View{});
		view_drawing_list.push_back(map_view);
	}
	else if (view_name == health_view_name_c){
		view_drawing_list.push_back(make_shared<Health_View>(Health_View{}));
	}	
	else if (view_name == amount_view_name_C){
		view_drawing_list.push_back(make_shared<Amount_View>(Amount_View{}));
	}
	else if (view_name == eta_view_name_c){
		view_drawing_list.push_back(make_shared<ETA_View>(ETA_View{}));
	}
	else { // look up object in model
		if (!Model::get_instance().is_name_in_use(view_name))
			throw Error {"No object of that name!"};
		// object must be either an agent or a structure
		shared_ptr<Sim_object> object_ptr;
		if (Model::get_instance().is_agent_present(view_name))
			object_ptr = Model::get_instance().get_agent_ptr(view_name);
		else // object is a structure
			object_ptr = Model::get_instance().get_structure_ptr(view_name);
		view_drawing_list.push_back(make_shared<Local_View>(
			Local_View{object_ptr->get_name(), object_ptr->get_location()}));
	}
	// add to map of names -> view pointers
	views.insert(make_pair(view_name, view_drawing_list.back()));
	// attach latest view to model
	Model::get_instance().attach(view_drawing_list.back());
}

void Controller::close_command(){
	string view_name;
	cin >> view_name;
	auto view_map_itr = views.find(view_name);
	if (view_map_itr == views.end())
		throw Error {"No view of that name is open!"};
	// check if it's the map view
	if (view_name == map_view_name_c){
		map_view.reset();
	}
	Model::get_instance().detach(view_map_itr->second);
	// remove from vector -> linear search since order is arbitrary.
	view_drawing_list.erase(find(view_drawing_list.begin(), view_drawing_list.end(), 
		view_map_itr->second));
	views.erase(view_map_itr);
}

void Controller::default_command(){
	if (!map_view)
		throw Error {map_not_open_msg_c};
	map_view->set_defaults();
}
	
void Controller::size_command(){
	if (!map_view)
		throw Error {map_not_open_msg_c};
	map_view->set_size(get_int_input());
}
	
void Controller::zoom_command(){
	if (!map_view)
		throw Error {map_not_open_msg_c};
	map_view->set_scale(get_double_input());
}

void Controller::pan_command(){
	if (!map_view)
		throw Error {map_not_open_msg_c};
	map_view->set_origin(get_Point_input());
}
	
void Controller::status_command(){
	Model::get_instance().describe();
}
	
void Controller::show_command(){
	for_each(view_drawing_list.begin(), view_drawing_list.end(),
		mem_fn(&View::draw));
}
	
void Controller::go_command(){
	Model::get_instance().update();
}

void Controller::build_command(){
	string name;
	cin >> name;
	if (name_is_invalid(name))
		throw Error {invalid_name_msg_c};
	string type;
	cin >> type;
	Model::get_instance().add_structure(create_structure(name, type, get_Point_input()));
}
	
void Controller::train_command(){
	string name;
	cin >> name;
	if (name_is_invalid(name))
		throw Error {invalid_name_msg_c};
	string type;
	cin >> type;
	Model::get_instance().add_agent(create_agent(name, type, get_Point_input()));
}

// returns true if name is invalid for a new object
bool name_is_invalid(const string& name){
	if (name.length() < minimum_name_length_c)
		return true;
	for (char c : name){
		if (!isalnum(c))
			return true;
	}
	return Model::get_instance().is_name_in_use(name);
}
	
void Controller::move_command(shared_ptr<Agent> agent){
	agent->move_to(get_Point_input());
}
	
void Controller::work_command(shared_ptr<Agent> agent){
	string source_name;
	cin >> source_name;
	shared_ptr<Structure> source = Model::get_instance().get_structure_ptr(source_name);
	string dest_name;
	cin >> dest_name;
	shared_ptr<Structure> dest = Model::get_instance().get_structure_ptr(dest_name);
	agent->start_working(source, dest);
}
	
void Controller::attack_command(shared_ptr<Agent> agent){
	string target_name;
	cin >> target_name;
	shared_ptr<Agent> target = Model::get_instance().get_agent_ptr(target_name);
	agent->start_attacking(target);
}
	
void Controller::stop_command(shared_ptr<Agent> agent){
	agent->stop();
}

// gets integer input from cin, throws exception if invalid
int get_int_input(){
	int input;
	cin >> input;
	if (!cin){
		cin.clear();
		throw Error {"Expected an integer!"};
	}
	return input;
}

// gets double input from cin, throws if invalid data found.
double get_double_input(){
	double input;
	cin >> input;
	if (!cin){
		cin.clear();
		throw Error {"Expected a double!"};
	}
	return input;
}

// attempts to read data for a point (two doubles) from cin
Point get_Point_input(){
	double x_coord = get_double_input();
	double y_coord = get_double_input();
	return Point {x_coord, y_coord};
}
