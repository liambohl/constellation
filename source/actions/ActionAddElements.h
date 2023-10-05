#pragma once

#include "Action.h"

#include "drawing/Element.h"


class ActionAddElements :
    public Action
{
public:
    ActionAddElements(std::vector<std::shared_ptr<Element>> elements) : elements(elements) {}

    void apply(Drawing& drawing);
    void undo(Drawing& drawing);

private:
    std::vector<std::shared_ptr<Element>> elements;
};
