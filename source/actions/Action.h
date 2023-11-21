#pragma once

#include "drawing/Drawing.h"


class Action
{
public:
    virtual void apply(Drawing& drawing) = 0;
    virtual void undo(Drawing& drawing) = 0;
};
