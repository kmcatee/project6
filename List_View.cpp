#include "List_View.h"
#include <iostream>

using std::cout; using std::endl;
using std::string;

void List_View::draw(){
	// should be using default floating point precision (whatever is globally set)
	cout << "--------------" << endl;
	for (const auto& map_obj : object_stats){
		cout << map_obj.first << ": " << map_obj.second << endl;
	}
	cout << "--------------" << endl;
}

void List_View::update_object(const string& name, double stat){
	object_stats[name] = stat;
}

void List_View::update_remove(const string& name){
	object_stats.erase(name);
}

