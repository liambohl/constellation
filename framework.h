// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define GDIPVER 0x0110

// Windows Header Files
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
//using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "comctl32.lib") 
#pragma comment(lib, "propsys.lib")
#pragma comment(lib, "shlwapi.lib") 

//#include <windef.h>
//#include <winbase.h>
//#include <wingdi.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

inline constexpr unsigned char operator "" _byte(unsigned long long arg) noexcept
{
    return static_cast<BYTE>(arg);
}