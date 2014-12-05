#ifndef STRUCTURE_H
#define STRUCTURE_H
#include "Sim_object.h"
#include "Geometry.h"
/* A Structure is a Sim_object with a location and interface to derived types */

class Structure : public Sim_object{

public:
	Structure(const std::string& name_, const Point location_);
	~Structure() = 0;  
		
	Point get_location() const
		{return location;}

	void update(){}

	// output information about the current state
	void describe() const;

	// ask model to notify views of current state
    void broadcast_current_state() override;	
    
	// fat interface for derived types	
	virtual double withdraw(double amount_to_get)
		{return 0.0;}
	virtual void deposit(double amount_to_give){}

private:
	Point location;
};

#endif
