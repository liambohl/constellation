#include "Initializer.h"


Initializer::Initializer() {
    Gdiplus::GdiplusStartupInput startupInput;
    Gdiplus::GdiplusStartup(&m_token, &startupInput, NULL);
    // TODO: check function return value

    CoInitializeEx(NULL, COINIT_MULTITHREADED);
}

Initializer::~Initializer() {
    if (m_token)
        Gdiplus::GdiplusShutdown(m_token);
    CoUninitialize();
}
