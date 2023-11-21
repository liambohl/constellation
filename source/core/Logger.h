#pragma once

#include <fstream>
#include <ios>
#include <map>
#include <ostream>
#include <streambuf>
#include <string>
#include <windows.h>


class Logger
{
public:
    static Logger* get_instance();

    void log_message(UINT message);

    // shove wide chars into chars and hope they are part of Unicode
    Logger& operator<< (wchar_t* val);
    // handle ostream manipulators like std::endl
    Logger& operator<< (std::ostream& (*manip)(std::ostream&));

    template<typename T>
    Logger& operator<< (T val) { file << val; return *this; }

private:
    static const char* filename;
    static const std::map<int, const char*> message_codes;

    Logger();
    ~Logger();

    static Logger* instance;

    std::ofstream file;
};

