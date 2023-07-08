#pragma once
#include "framework.h"

// Credit for this class to zett42 on Stack Overflow
// https://stackoverflow.com/a/51541939/8218402

namespace Constellation {

    class GdiPlusInitializer
    {
    public:
        GdiPlusInitializer()
        {
            Gdiplus::GdiplusStartupInput startupInput;
            Gdiplus::GdiplusStartup(&m_token, &startupInput, NULL);
            // TODO: check function return value
        }

        ~GdiPlusInitializer()
        {
            if (m_token)
                Gdiplus::GdiplusShutdown(m_token);
        }

        // Class is non-copyable.
        GdiPlusInitializer(const GdiPlusInitializer&) = delete;
        GdiPlusInitializer& operator=(const GdiPlusInitializer&) = delete;

    private:
        ULONG_PTR m_token = 0;
    };

}