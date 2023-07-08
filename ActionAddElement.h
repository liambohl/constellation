#pragma once
#include "Action.h"
#include "Element.h"

namespace Constellation {

    class ActionAddElement :
        public Action
    {
    public:
        ActionAddElement(Element* element) : element(element) {}
        ~ActionAddElement();

        void apply(Drawing& drawing);
        void undo(Drawing& drawing);

    private:
        Element* element;
    };

}
