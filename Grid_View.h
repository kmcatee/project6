#ifndef GRID_VIEW_H
#define GRID_VIEW_H
#include "View.h"
#include <vector>
#include <map>

// minimum and maximum grid sizes. 
const int max_size_c {30}; 
const int min_size_c {7}; 

// A View comprised of names plotted on a grid at various locaitons
class Grid_View : public View {
public:
	void draw() override;
	void update_location(const std::string& name, Point location) override;
	void update_remove(const std::string& name);
	

protected:
	Grid_View(int size_, double scale_, Point origin_);
	// if the size is out of bounds will throw Error("New map size is too big!")
	// or Error("New map size is too small!")
	void set_size(int size_);
	
	// If scale is not postive, will throw Error("New map scale must be positive!");
	void set_scale(double scale_);
	
	// any values are legal for the origin
	void set_origin(Point origin_);
	// centers grid on point provided
	void set_center(Point center);
	void clear();
	// prints size, scale, origin
	void print_grid_parameters();
	// returns vector of all objects registered with grid but not currently displayed
	std::vector<std::string> get_objects_off_map();

private:
	int size;
	double scale;
	Point origin;
	std::map<std::string, Point> objects;
	// helper function that maps real-number coordinates into discrete array subscripts
	bool get_subscripts(int &ix, int& iy, Point location);
};

#endif
