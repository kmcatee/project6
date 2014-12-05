#ifndef AMOUNT_VIEW_H
#define AMOUNT_VIEW_H
#include "List_View.h"
#include <string>

// A list of objects and their amount values
class Amount_View : public List_View {
public:
	Amount_View(){}
	void draw() override;
	void update_amount(const std::string& name, double amount) override;
};

#endif