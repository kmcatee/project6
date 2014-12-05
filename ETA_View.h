#ifndef ETA_VIEW_H
#define ETA_VIEW_H
#include "List_View.h"

// A list of objects and their health values
class ETA_View : public List_View {
public:
	ETA_View(){}
	void draw() override;
	void update_eta(const std::string& name, int turns) override;
	void update_stop_moving(const std::string& name) override;
};

#endif
