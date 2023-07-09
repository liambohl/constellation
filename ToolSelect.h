#pragma once
#include "Tool.h"

namespace Constellation {

    class ToolSelect :
        public Tool
    {
    public:
        ToolSelect(Drawing& drawing) : Tool(drawing) { }

        Action* handle_mouse_event(UINT message, WPARAM wParam, LPARAM lParam);
        void draw(Canvas& canvas);
    };

}
