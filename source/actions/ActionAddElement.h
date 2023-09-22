#pragma once

#include "Action.h"

#include "drawing/Element.h"


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
