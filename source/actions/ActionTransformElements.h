#pragma once

#include "Action.h"


class ActionTransformElements :
    public Action
{
public:
    ActionTransformElements(std::vector<std::shared_ptr<Element>> elements, Gdiplus::Matrix* transform):
        elements(elements),
        transform(transform)
    {}
    ~ActionTransformElements();

    void apply(Drawing& drawing);
    void undo(Drawing& drawing);

private:
    std::vector<std::shared_ptr<Element>> elements;
    Gdiplus::Matrix* transform;
};

