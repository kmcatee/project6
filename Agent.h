#ifndef AGENT_H
#define AGENT_H
#include "Sim_object.h"
#include "Moving_object.h"
#include <memory>
/* 
Agents are a kind of Sim_object, and privately inherit from Moving_object.
Agents can be commanded to move to a destination. Agents have a health, which
is decreased when they take a hit. If the Agent's health > 0, it is alive.
If its heath <= 0, it starts dying, then on subsequent updates, 
it becomes dead, and finally disappearing.
*/

class Structure;

class Agent: public Sim_object, public std::enable_shared_from_this<Agent> {
public:
	// return true if this agent is Alive or Disappearing
	bool is_alive() const
		{return (alive_state == Alive_e::Alive);}

	// return this Agent's location
	Point get_location() const override
		{return movement.get_current_location();} 

	// return true if this Agent is in motion
	bool is_moving() const
		{return movement.is_currently_moving();}
	
	// tell this Agent to start moving to location destination_
	virtual void move_to(Point destination_);

	// tell this Agent to stop its activity
	virtual void stop();

	// Tell this Agent to accept a hit from an attack of a specified strength
	// The attacking Agent identifies itself with its this pointer.
	// A derived class can override this function.
	// The function lose_health is called to handle the effect of the attack.
	virtual void take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr);
	
	// update the moving state and Agent state of this object.
	void update() override;
	
	// output information about the current state
	void describe() const override;
	
	// ask Model to broadcast our current state to all Views
	void broadcast_current_state() override;

	/* Fat Interface for derived classes */
	// Throws exception that an Agent cannot work.
	virtual void start_working(std::shared_ptr<Structure>, std::shared_ptr<Structure>);

	// Throws exception that an Agent cannot attack.
	virtual void start_attacking(std::shared_ptr<Agent>);

	// Trhows exception that an Agent cannot protect;
	virtual void start_protecting(std::shared_ptr<Agent>);

protected:
	Agent(const std::string& name_, Point location_);

	// calculate loss of health due to hit.
	// if health decreases to zero or negative, Agent state becomes Dying, and any movement is stopped.
	void lose_health(int attack_strength);

private:
	Moving_object movement;
	int health;
	enum class Alive_e{
		Alive, Dead
	}alive_state;
	// Will handle computations for notifying the Model that this is moving
	void notify_moving_helper();
};

#endif
