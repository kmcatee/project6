#ifndef GUARD_H
#define GUARD_H
#include "Warrior.h"
#include <memory>
//Guards are Warriors that devote their lives to a specific Ageng
//and protect and follow them until a demise or betrayal

class Guard : public Warrior {
public:
	Guard(const std::string& name_, Point location_);
	void update() override;
	void start_attacking(std::shared_ptr<Agent> target_ptr) override;
	void take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr) override;
	void describe() const override;
	void start_protecting(std::shared_ptr<Agent>);
private:
	void print_attack_msg() const override;
	// charge is the Agent that Guard is protecting
	std::weak_ptr<Agent> charge;

};

#endif
