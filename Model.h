#ifndef MODEL_H
#define MODEL_H
#include "Geometry.h"
#include <memory>
#include <map>
#include <string>
#include <vector>
/*
Model is part of a simplified Model-View-Controller pattern.
Model keeps track of the Sim_objects in our little world. It is the only
component that knows how many Structures and Agents there are, but it does not
know about any of their derived classes, nor which Agents are of what kind of Agent. 
It has facilities for looking up objects by name, and removing Agents.  When
created, it creates an initial group of Structures and Agents using the Structure_factory
and Agent_factory.
Finally, it keeps the system's time.

Controller tells Model what to do; Model in turn tells the objects what do, and
when asked to do so by an object, tells all the Views whenever anything changes that might be relevant.
Model also provides facilities for looking up objects given their name.

Notice how only the Standard Library headers need to be included - reduced coupling!

*/

// forward declarations for objects and View:
class Sim_object;
class Structure;
class Agent;
class View; 
 
class Model {
public:

	// singleton accessor
	static Model& get_instance();

	// return the current time
	int get_time() {return time;}

	// is name already in use for either agent or structure?
    // return true if the name matches the name of an existing agent or structure
	bool is_name_in_use(const std::string& name) const;

	// is there a structure with this name?
	bool is_structure_present(const std::string& name) const;
	// add a new structure; assumes none with the same name
	void add_structure(std::shared_ptr<Structure>);
	// will throw Error("Structure not found!") if no structure of that name
	std::shared_ptr<Structure> get_structure_ptr(const std::string& name) const;

	// returns structure nearest to agent or nullptr if there are no structures
	std::shared_ptr<Structure> get_nearest_structure(std::shared_ptr<Agent> agent) const;

	// is there an agent with this name?
	bool is_agent_present(const std::string& name) const;
	// add a new agent; assumes none with the same name
	void add_agent(std::shared_ptr<Agent>);
	// will throw Error("Agent not found!") if no agent of that name
	std::shared_ptr<Agent> get_agent_ptr(const std::string& name) const;

	// returns agent nearest to agent or nullptr if there are no agents
	std::shared_ptr<Agent> get_nearest_agent(std::shared_ptr<Agent> agent1, 
		std::shared_ptr<Agent> agent2 = nullptr) const;

	// tell all objects to describe themselves to the console
	void describe() const;
	// increment the time, and tell all objects to update themselves
	void update();	
	
	/* View services */
	// Attaching a View adds it to the container and causes it to be updated
    // with all current objects'location (or other state information.
	void attach(std::shared_ptr<View>);
	// Detach the View by discarding the supplied pointer from the container of Views
    // - no updates sent to it thereafter.
	void detach(std::shared_ptr<View>);
    // notify the views about an object's location
	void notify_location(const std::string& name, Point location);
	// notify the views about an object's health
	void notify_health(const std::string& name, int health);
	// notify the views about an object's amount (of food)
	void notify_amount(const std::string& name, double amount);
	// notify the views that an object is now moving
	void notify_eta(const std::string& name, int turns);
	// notify the views that an object is now not moving
	void notify_stop_moving(const std::string& name);
	// notify the views that an object is now gone
	void notify_gone(const std::string& name);
	// remove this agent from all containers.
	void remove_agent(std::shared_ptr<Agent> agent);
	
private:
	int time;
	std::map<std::string, std::shared_ptr<Sim_object>> objects;
	std::map<std::string, std::shared_ptr<Structure>> structures;
	std::map<std::string, std::shared_ptr<Agent>> agents;
	//views have no sorting criterion. 
	std::vector<std::shared_ptr<View>> views;

	// create the initial objects
	Model();
	// private dtor - singleton
	~Model(){} 
	// construct and insert a structure or agent. Don't broadcast state.
	void structure_insert_helper(std::shared_ptr<Structure> s_ptr);
	void agent_insert_helper(std::shared_ptr<Agent> a_ptr);

	// disallow copy/move construction or assignment
	Model(const Model&) = delete;
	Model& operator= (const Model&)  = delete;
	Model(Model&&) = delete;
	Model& operator= (Model&&) = delete;
};

#endif
