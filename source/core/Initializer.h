#pragma once

#include "framework.h"


// Credit for this class to zett42 on Stack Overflow
// https://stackoverflow.com/a/51541939/8218402

class Initializer
{
public:
    Initializer();
    ~Initializer();

    // Class is non-copyable.
    Initializer(const Initializer&) = delete;
    Initializer& operator=(const Initializer&) = delete;

private:
    ULONG_PTR m_token = 0;
};
