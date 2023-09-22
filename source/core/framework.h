#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define GDIPVER 0x0110

// Windows Header Files
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "comctl32.lib") 
#pragma comment(lib, "propsys.lib")
#pragma comment(lib, "shlwapi.lib") 

inline constexpr unsigned char operator "" _byte(unsigned long long arg) noexcept
{
    return static_cast<BYTE>(arg);
}