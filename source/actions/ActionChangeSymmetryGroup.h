#pragma once
#include "Action.h"
#include "drawing/symmetry/SymmetryGroup.h"

class ActionChangeSymmetryGroup :
    public Action
{
public:
    ActionChangeSymmetryGroup(std::shared_ptr<SymmetryGroup> old_group, std::shared_ptr<SymmetryGroup> new_group) :
        new_group(new_group), old_group(old_group) {}

    void apply(Drawing& drawing);
    void undo(Drawing& drawing);

private:
    std::shared_ptr<SymmetryGroup> new_group;
    std::shared_ptr<SymmetryGroup> old_group;
};

