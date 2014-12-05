#include "Structure_factory.h"
#include "Farm.h"
#include "Town_Hall.h"
#include "Geometry.h"
#include "Utility.h"

using std::string;
using std::shared_ptr;

const string farm_type_name_c = "Farm";
const string town_hall_type_name_c = "Town_Hall";

shared_ptr<Structure> create_structure(const string& name, const string& type, Point location){
	if (type == farm_type_name_c)
		return shared_ptr<Farm>(new Farm{name, location});
	else if (type == town_hall_type_name_c)
		return shared_ptr<Town_Hall>(new Town_Hall{name, location});
	else 
		throw Error {"Trying to create structure of unknown type!"};
}
