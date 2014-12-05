#ifndef LIST_VIEW_H
#define LIST_VIEW_H
#include "View.h"
#include <map>
#include <string>

// A list of names and datum associated with those names.
class List_View : public View{
public:
	void draw() override;
	void update_remove(const std::string& name) override;
protected:
	List_View(){}
	void update_object(const std::string& name, double stat);
	bool no_objects() const
	{return object_stats.empty();}
private:
	std::map<std::string, double> object_stats;
};

#endif
