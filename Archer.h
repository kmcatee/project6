#ifndef ARCHER_H
#define ARCHER_H
#include "Warrior.h"

// Archer will automatically attack any Agent in range, and will automatically
// run away to the nearest structure when attacked. Can otherwise be commanded
// to move and attack.
class Archer : public Warrior {
public:
	Archer(const std::string& name_, Point location_);
	void update() override;
	void take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr) override;
	void describe() const override;
private:
	void print_attack_msg() const override;
};

#endif
