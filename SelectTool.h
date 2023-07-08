#pragma once
#include "Tool.h"

namespace Constellation {

    class SelectTool :
        public Tool
    {
    public:
        SelectTool(Drawing& drawing) : Tool(drawing) { }

        void handleMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);
        void draw(Canvas& canvas);
    };

}
