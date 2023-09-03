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
	const char* filename = "../errors.log";
	static const std::map<int, const char*> message_codes;
	static Logger* instance;

	Logger();
	~Logger();

	std::ofstream file;

public:
	static Logger* get_instance();
	void log_message(UINT message);

	// shove wide chars into chars and hope they are part of Unicode
	Logger& operator<< (wchar_t* val);
	// handle ostream manipulators like std::endl
	Logger& operator<< (std::ostream& (*manip)(std::ostream&));

	template<typename T>
	Logger& operator<< (T val) { file << val; return *this; }
};

