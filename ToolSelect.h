#pragma once
#include "Tool.h"

class ToolSelect :
    public Tool
{
public:
    ToolSelect(Defaults& defaults) : Tool(defaults) { }

    Action* handle_mouse_event(UINT message, WPARAM wParam, LPARAM lParam);
    void draw(Canvas& canvas);
};
