#ifndef MAP_VIEW_H
#define MAP_VIEW_H
#include "Grid_View.h"

// Adjustable Grid-based View.
class Map_View : public Grid_View {
public:
	// default constructor sets the default size, scale, and origin
	Map_View(); 
	
	// prints out the current map
	void draw() override;

	// makes setter functions publicly available
	void set_size(int size_){Grid_View::set_size(size_);}
	void set_scale(double scale_){Grid_View::set_scale(scale_);}
	void set_origin(Point origin_){Grid_View::set_origin(origin_);}
	
	// set the parameters to the default values
	void set_defaults();

};

#endif