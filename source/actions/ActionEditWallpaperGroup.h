#pragma once

#include "Action.h"


class ActionEditWallpaperGroup :
    public Action
{
public:
    enum VECTOR { EDIT_V1, EDIT_V2 };

    ActionEditWallpaperGroup(VECTOR vector, Gdiplus::PointF vector_initial, Gdiplus::PointF vector_new) :
        vector(vector), vector_initial(vector_initial), vector_new(vector_new)
    { }

    virtual void apply(Drawing& drawing) override;
    virtual void undo(Drawing& drawing) override;

private:
    VECTOR vector = EDIT_V1;
    Gdiplus::PointF vector_initial, vector_new;
};

