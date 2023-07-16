#pragma once
#include "Action.h"
#include "Element.h"

namespace Constellation {

    class ActionAddElement :
        public Action
    {
    public:
        ActionAddElement(std::shared_ptr<Element> element) : element(element) {}

        void apply(Drawing& drawing);
        void undo(Drawing& drawing);

    private:
        std::shared_ptr<Element> element;
    };

}
