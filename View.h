#ifndef VIEW_H
#define VIEW_H
#include "Geometry.h"

class View{
public:
	virtual ~View(){}
	// Save the supplied name and location for future use in a draw() call
	// If the name is already present,the new location replaces the previous one.
	virtual void update_location(const std::string& name, Point location){}
	// save the supplied name and health amount for future use in a draw() call
	virtual void update_health(const std::string& name, int health){}
	// save the supplied name and amount for use in a future draw() call.
	virtual void update_amount(const std::string& name, double amount){}
	// save the supplied name and number of turns for use in a future draw() call.
	virtual void update_eta(const std::string& name, int turns){}

	// Inform the view that that object of that name is no longer moving
	virtual void update_stop_moving(const std::string& name){}
	// Remove the name and its location; no error if the name is not present.
	virtual void update_remove(const std::string& name) = 0;
	
	// prints the contents of the view
	virtual void draw() = 0;

};

#endif