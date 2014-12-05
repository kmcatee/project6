#ifndef WARRIOR_H
#define WARRIOR_H
#include "Agent.h"
#include <memory>

class Warrior : public Agent {
public:
	Warrior(const std::string& name_, Point location_, int attack_str_,
			double attack_range_);
	void update() override;
	// Make this Warrior start attacking the target Agent.
	// Throws an exception if the target is the same as this Agent,
	// is out of range, or is not alive.
	void start_attacking(std::shared_ptr<Agent> target_ptr) override;
	void take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr) override;
	// Overrides Agent's stop to print a message
	void stop() override;
	void describe() const override;
protected:
	// print the message for an attack action
	virtual void print_attack_msg() const = 0;
	// requests a change in current_target
	void set_target(std::shared_ptr<Agent> new_target);

	bool is_attacking() const {return attacking_state;}
	// returns true if target is out of attack range
	bool target_out_of_range(std::shared_ptr<Agent> target_ptr);
private:
	int attack_str;
	double attack_range;
	bool attacking_state;
	std::weak_ptr<Agent> current_target;
	
};

#endif

