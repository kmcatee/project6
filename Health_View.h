#ifndef HEALTH_VIEW_H
#define HEALTH_VIEW_H
#include "List_View.h"

// A list of objects and their health values
class Health_View : public List_View {
public:
	Health_View(){}
	void draw() override;
	void update_health(const std::string& name, int health) override;
};

#endif
