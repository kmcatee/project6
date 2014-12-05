#include "Map_View.h"
#include <iostream>
#include <string>
#include <vector>
#include <cassert>

using std::cout; using std::endl;
using std::string;
using std::vector;


const double default_map_view_scale_c {2.0};
const Point default_map_view_origin_c {-10.0, -10.0};
const int default_map_view_size_c {25};

Map_View::Map_View() :
	Grid_View(default_map_view_size_c, default_map_view_scale_c,
		default_map_view_origin_c)
	{
		// requirements for size and scale from Grid_View:
		assert(default_map_view_size_c >= min_size_c && 
			default_map_view_size_c <= max_size_c);
		assert(default_map_view_scale_c > 0.0);
	}

void Map_View::draw(){
	// print out size, scale, origin
	print_grid_parameters();
	vector<string> objects_off_map = get_objects_off_map();
	int num_off_grid = static_cast<int>(objects_off_map.size());
	for (int i = 0; i < num_off_grid; ++i){
		if (i) // output before all names except the first
			cout << ", "; 
		cout << objects_off_map[i];
	}
	if (num_off_grid)
		cout << " outside the map" << endl;
	Grid_View::draw();
}

void Map_View::set_defaults(){
	Grid_View::set_size(default_map_view_size_c);
	Grid_View::set_scale(default_map_view_scale_c);
	Grid_View::set_origin(default_map_view_origin_c);
}

