#pragma once

#include <string>
#include <map>
#include <fstream>
#include <ostream>
#include <streambuf>
#include <ios>
#include <windows.h>

class Logger
{
private:
	static const PCSTR filename;
	static const std::map<int, PCSTR> message_codes;
	static Logger* instance;

	Logger();
	~Logger();

	std::ofstream file;

public:
	static Logger* get_instance();
	void log(std::string s);
	void log_message(UINT message);

	template<typename T>
	std::ostream& operator<< (T val) { return file << val; }
};

