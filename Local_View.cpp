#include "Local_View.h"
#include <iostream>
#include <string>
#include <cassert>

using std::cout; using std::endl;
using std::string;

const double default_local_view_scale_c {2.0};
const int default_local_view_size_c {9};


Local_View::Local_View(const string& central_object_, Point location_) :
	Grid_View(default_local_view_size_c, default_local_view_scale_c, 
		Point(0,0)), central_object(central_object_)
{
	// need to fully construct Grid_View before asking it to adjust its origin
	set_center(location_);
	// requirements for size and scale from Grid_View:
	assert(default_local_view_size_c >= min_size_c && 
			default_local_view_size_c <= max_size_c);
	assert(default_local_view_scale_c > 0.0);
}
	
void Local_View::draw(){
	cout << "Local view for: " << central_object << endl;
	Grid_View::draw();
}
	
void Local_View::update_location(const string& name, Point location){
	Grid_View::update_location(name, location);
	if (name == central_object)
		set_center(location);
}
