#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <string>
#include <map>
#include <memory>
#include <vector>
/* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/

//forward declarations
class View;
class Map_View;
class Agent;

class Controller {
public:	
	Controller();

	// create View object, run the program by acccepting user commands, then destroy View object
	void run();

private:
	// containers to hold member function pointers
	std::map<std::string, void(Controller::*)()> program_commands;
	std::map<std::string, void(Controller::*)(std::shared_ptr<Agent>)> agent_commands;
	// container of views:
	std::map<std::string, std::shared_ptr<View>> views;
	// List with order to draw views in.
	std::vector<std::shared_ptr<View>> view_drawing_list;
	// specific access to Map_View if it is active.
	std::shared_ptr<Map_View> map_view; 

	// Program-wide commands:
	// specific-view commands:
	void open_command();
	void close_command();
	
	// commands that apply only to map view
	void default_command();
	void size_command();
	void zoom_command();
	void pan_command();
	// general commands:
	void show_command();
	void status_command();
	void go_command();
	void build_command();
	void train_command();
	// Agent-based commands:
	void move_command(std::shared_ptr<Agent> agent);
	void work_command(std::shared_ptr<Agent> agent);
	void attack_command(std::shared_ptr<Agent> agent);
	void stop_command(std::shared_ptr<Agent> agent);

};

#endif
