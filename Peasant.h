#ifndef PEASANT_H
#define PEASANT_H
#include "Agent.h"
#include <memory>
/* 
A Peasant is an Agent that can move food between Structures. It can be commanded to
start_working, whereupon it moves to the source, picks up food, returns to destination,
deposits the food, returns to source.  If picks up zero food at the source, it waits there
and tries again on the next update. 
If commanded to move_to somewhere, it stops working, and goes there.
*/

class Peasant : public Agent {
public:
	Peasant(const std::string& name_, Point location_);

	// implement Peasant behavior
	void update() override;
	
	// overridden to suspend working behavior
    void move_to(Point dest) override;
	
	// stop moving and working
	void stop() override;

	// starts the working process
	// Throws an exception if the source is the same as the destination.
	void start_working(std::shared_ptr<Structure> source_, std::shared_ptr<Structure> destination_) override;

	// output information about the current state
	void describe() const override;

	// ask model to notify views of current state
	void broadcast_current_state() override;

private:
	std::shared_ptr<Structure> source;
	std::shared_ptr<Structure> destination;
	double food_carried;
	enum class Work_phase_e{
		Idle, Inbound, Collecting, Outbound, Depositing
	}work_state;
};

#endif
