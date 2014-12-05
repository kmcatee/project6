#include "Model.h"
#include "Structure_factory.h"
#include "Agent_factory.h"
#include "Structure.h"
#include "Agent.h"
#include "View.h"
#include "Utility.h"
#include <algorithm>
#include <utility>
#include <functional>

using std::map;
using std::string;
using std::vector;
using std::for_each;
using std::bind; 
using std::find;
using namespace std::placeholders;
using std::make_pair;
using std::shared_ptr;
using std::min_element;

using Object_map_t = map<string, shared_ptr<Sim_object>>;
using Structure_map_t = map<string, shared_ptr<Structure>>;
using Agent_map_t = map<string, shared_ptr<Agent>>;


Model& Model::get_instance(){
	static Model model; //Meyers singleton pattern
	return model;
}

Model::Model() :
	time(0)
{
	// Don't broadcast_current_state until entire Model is constructed
	structure_insert_helper(create_structure("Rivendale", "Farm", Point(10., 10.)));
	structure_insert_helper(create_structure("Sunnybrook", "Farm", Point(0., 30.)));
	structure_insert_helper(create_structure("Shire", "Town_Hall", Point(20., 20.)));
	structure_insert_helper(create_structure("Paduca", "Town_Hall", Point(30., 30.)));
	agent_insert_helper(create_agent("Pippin", "Peasant", Point(5., 10.)));
	agent_insert_helper(create_agent("Merry", "Peasant", Point(0., 25.)));
	agent_insert_helper(create_agent("Zug", "Soldier", Point(20., 30.)));
	agent_insert_helper(create_agent("Bug", "Soldier", Point(15., 20.)));
	agent_insert_helper(create_agent("Iriel", "Archer", Point(20., 38.)));
}

bool Model::is_name_in_use(const string& name) const{
	return (objects.find(name) != objects.end());
}

bool Model::is_structure_present(const string& name) const{
	return (structures.find(name) != structures.end());
}

void Model::add_structure(shared_ptr<Structure> structure){
	structure_insert_helper(structure);
	structure->broadcast_current_state();
}

void Model::structure_insert_helper(shared_ptr<Structure> s_ptr){
	objects.insert(make_pair(s_ptr->get_name(), s_ptr));
	structures.insert(make_pair(s_ptr->get_name(), s_ptr));
}

shared_ptr<Structure> Model::get_structure_ptr(const string& name) const{
	auto map_it = structures.find(name);
	if (map_it == structures.end())
		throw Error {"Structure not found!"};
	return map_it->second;
}

// Comparison function object to define order of Sim_objects in terms of distances from a 
// supplied Sim_object. 
// A name matching the predicate should be set to the 'greatest' possible value.
// Otherwise, ties in distance are broken by lexicographic order of names.
class Sim_object_dist_comp {
public:
	Sim_object_dist_comp(shared_ptr<Sim_object> object) : 
		location(object->get_location()), name(object->get_name()) {}
	bool operator()(Object_map_t::value_type s1, Object_map_t::value_type s2) const{
		if (s1.second->get_name() == name)
			return false;
		if (s2.second->get_name() == name)
			return true;
		double dist1 = cartesian_distance(s1.second->get_location(), location); 
		double dist2 = cartesian_distance(s2.second->get_location(), location);
		if (dist1 < dist2)
			return true;
		else if (dist2 < dist1)
			return false;
		return (s1.second->get_name() < s2.second->get_name());
	}
private:
	Point location;
	string name;
};

shared_ptr<Structure> Model::get_nearest_structure(shared_ptr<Agent> agent) const{
	auto min = min_element(structures.begin(), structures.end(),
		Sim_object_dist_comp(agent));
	if (min != structures.end())
		return min->second;
	return nullptr;
}

bool Model::is_agent_present(const string& name) const{
	return (agents.find(name) != agents.end());
}

void Model::add_agent(shared_ptr<Agent> agent){
	agent_insert_helper(agent);
	agent->broadcast_current_state();
}
	
void Model::agent_insert_helper(shared_ptr<Agent> a_ptr){
	objects.insert(make_pair(a_ptr->get_name(), a_ptr));
	agents.insert(make_pair(a_ptr->get_name(), a_ptr));
}

shared_ptr<Agent> Model::get_agent_ptr(const std::string& name) const{
	auto map_it = agents.find(name);
	if (map_it == agents.end())
		throw Error {"Agent not found!"};
	return map_it->second;
}

shared_ptr<Agent> Model::get_nearest_agent(shared_ptr<Agent> agent1, std::shared_ptr<Agent> agent2) const {
	auto min = min_element(agents.begin(), agents.end(),
		Sim_object_dist_comp(agent1));
	if (min != agents.end() && min->second != agent1)
		return min->second;
	return nullptr;
}

void Model::describe() const{
	for_each(objects.begin(), objects.end(),
		bind(&Sim_object::describe,
			bind(&Object_map_t::value_type::second, _1)));
}

void Model::update(){
	++time;
	for_each(objects.begin(), objects.end(), 
		bind(&Sim_object::update,
			bind(&Object_map_t::value_type::second, _1)));
}

void Model::attach(shared_ptr<View> view_ptr){
	// call broadcast_current_state on each Sim_object
	views.push_back(view_ptr);
	for_each(objects.begin(), objects.end(),
		bind(&Sim_object::broadcast_current_state,
			bind(&Object_map_t::value_type::second, _1)));
}

void Model::detach(shared_ptr<View> view_ptr){
	// just remove view_ptr from views
	auto view_it = find(views.begin(), views.end(), view_ptr);
	if (view_it != views.end())
		views.erase(view_it);
}

void Model::notify_location(const string& name, Point location){
	for_each(views.begin(), views.end(),
		bind(&View::update_location, _1, name, location));
}

void Model::notify_health(const string& name, int health){
	for_each(views.begin(), views.end(),
		bind(&View::update_health, _1, name, health));
}

void Model::notify_amount(const string& name, double amount){
	for_each(views.begin(), views.end(),
		bind(&View::update_amount, _1, name, amount));
}

void Model::notify_eta(const std::string& name, int turns){
	for_each(views.begin(), views.end(),
		bind(&View::update_eta, _1, name, turns));
}

void Model::notify_stop_moving(const std::string& name){
	for_each(views.begin(), views.end(),
		bind(&View::update_stop_moving, _1, name));
}

void Model::notify_gone(const string& name){
	for_each(views.begin(), views.end(),
		bind(&View::update_remove, _1, name));
}

void Model::remove_agent(shared_ptr<Agent> agent){
	objects.erase(agent->get_name());
	agents.erase(agent->get_name());
}
