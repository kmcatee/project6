#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <exception>

/* Utility declarations, functions, and classes used by other modules */

// a simple class for error exceptions that inherits from std::exception
class Error : public std::exception {
public:
	Error(const std::string& in_msg = "") :
		msg(in_msg)
		{}
    const char* what() const noexcept override {return msg.c_str();}
private:
	const std::string msg;
};
#endif

