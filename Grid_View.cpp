#include "Grid_View.h"
#include "Utility.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iterator>

using std::string;
using std::cout; using std::endl;
using std::ios_base;
using std::ios;
using std::setw;
using std::fixed; using std::setprecision;
using std::vector;
using std::floor;
using std::copy;
using std::ostream_iterator;



const char* const empty_cell_c {". "};
const char* const multiple_objects_cell_c {"* "};

Grid_View::Grid_View(int size_, double scale_, Point origin_) :
	size(size_), scale(scale_), origin(origin_)
{}


void Grid_View::draw(){
	vector<vector<string>> grid(size, vector<string>(size, string{empty_cell_c}));
	//populate grid with objects
	for (auto& map_obj : objects){
		int x, y;	
		if (get_subscripts(x, y, map_obj.second)){
			if (grid[y][x] == empty_cell_c){
				grid[y][x] = map_obj.first.substr(0, 2); 
				// first two characters from object name
			} 
			else {
				// more than one object plotted in same cell
				grid[y][x] = string(multiple_objects_cell_c);
			}	
		}
	}
	//save old stream settings
	ios_base::fmtflags old_flags = cout.flags();
	int old_precision = cout.precision();
	// change flags to needed settings
	cout.setf(ios::fixed);
	cout.precision(0); 
	// print Y-value legend and grid contents
	for (int i = 0; i < size; ++i){
		// printing from highest y-value to smallest (top-down)
		if ((size - i - 1) % 3 == 0)
			cout << setw(4) << ((size - i - 1) * scale) + origin.y << " ";
		else 
			cout << "     ";
		// print this row's contents
		copy(grid[size - i - 1].begin(), grid[size - i - 1].end(),
			ostream_iterator<string>(cout));
		cout << endl;
	}
	// print X-value legend
	for (int i = 0; i < size; ++i){
		// printing from smallest x-value to largest
		if (i % 3 == 0)
			cout << "  " << setw(4) << (i * scale) + origin.x;
	}
	cout << endl;
	//restore old stream settings
	cout.flags(old_flags);
	cout.precision(old_precision);
}

void Grid_View::clear(){
	objects.clear();
}

void Grid_View::set_size(int size_){
	if (size_ > max_size_c)
		throw Error {"New map size is too big!"};
	if (size_ < min_size_c)
		throw Error {"New map size is too small!"};
	size = size_;
}

void Grid_View::set_scale(double scale_){
	if (scale_ <= 0.0)
		throw Error {"New map scale must be positive!"};
	scale = scale_;
}

void Grid_View::set_origin(Point origin_){
	origin = origin_;
}

void Grid_View::set_center(Point center){
	origin.x = (center.x - (size / 2. * scale));
	origin.y = (center.y - (size / 2. * scale));
}

// Calculate the cell subscripts corresponding to the supplied location parameter, 
// using the current size, scale, and origin of the display. 
// This function assumes that origin is a  member variable of type Point, 
// scale is a double value, and size is an integer for the number of rows/columns 
// currently being used for the grid.
// Return true if the location is within the grid, false if not
bool Grid_View::get_subscripts(int &ix, int &iy, Point location)
{
	// adjust with origin and scale
	Cartesian_vector subscripts = (location - origin) / scale;
	// truncate coordinates to integer after taking the floor
	// floor function will return the largest integer smaller than the supplied value
	// even for negative values, so -0.05 => -1., which will be outside the array.
	ix = int(floor(subscripts.delta_x));
	iy = int(floor(subscripts.delta_y));
	// if out of range, return false
	if ((ix < 0) || (ix >= size) || (iy < 0) || (iy >= size)) {
		return false;
		}
	else
		return true;
}

void Grid_View::update_location(const string& name, Point location){
	objects[name] = location;
	// unconditionally want this location stored at this name
}

void Grid_View::update_remove(const string& name){
	objects.erase(name);
}

void Grid_View::print_grid_parameters(){
	cout << "Display size: " << size << ", scale: " << scale << ", origin: " << origin << endl;
}

vector<string> Grid_View::get_objects_off_map(){
	vector<string> off_map;
	int x, y;
	for (const auto& map_obj : objects){
		if (!get_subscripts(x, y, map_obj.second))
			off_map.push_back(map_obj.first);
	}
	return off_map;
}

