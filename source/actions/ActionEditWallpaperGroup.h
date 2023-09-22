#pragma once

#include "Action.h"


class ActionEditWallpaperGroup :
    public Action
{
public:
	enum VECTOR { EDIT_V1, EDIT_V2 };

	ActionEditWallpaperGroup(VECTOR vector, float old_x, float old_y, float new_x, float new_y) :
		vector(vector), old_x(old_x), old_y(old_y), new_x(new_x), new_y(new_y)
	{ }

	virtual void apply(Drawing& drawing) override;
	virtual void undo(Drawing& drawing) override;

private:
	VECTOR vector = EDIT_V1;
	float old_x, old_y, new_x, new_y;
};

