#ifndef LOCAL_VIEW_H
#define LOCAL_VIEW_H
#include "Grid_View.h"
#include <string>

// Grid View centered around a particular object.
class Local_View : public Grid_View {
public:
	Local_View(const std::string& central_object_, Point location_);
	void draw() override;
	void update_location(const std::string& name, Point location) override;
private:
	std::string central_object;
}; 

#endif