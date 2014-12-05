#include "Agent_factory.h"
#include "Peasant.h"
#include "Soldier.h"
#include "Archer.h"
#include "Geometry.h"
#include "Utility.h"

using std::string;
using std::shared_ptr;

const string peasant_type_name_c = "Peasant";
const string soldier_type_name_c = "Soldier";
const string archer_type_name_c = "Archer";

shared_ptr<Agent> create_agent(const string& name, const string& type, Point location){
	if (type == peasant_type_name_c)
		return shared_ptr<Agent>(new Peasant{name, location});
	else if (type == soldier_type_name_c)
		return shared_ptr<Agent>(new Soldier{name, location});
	else if (type == archer_type_name_c)
		return shared_ptr<Agent>(new Archer{name, location});
	else 
		throw Error {"Trying to create agent of unknown type!"};
}

