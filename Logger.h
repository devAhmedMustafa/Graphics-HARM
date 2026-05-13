#pragma once

#include <iostream>
#include <ostream>

class Logger
{
public:
	std::ostream& operator<<(const std::string& msg) {
		return std::cout << "[Log]__" << msg;
	}
};

