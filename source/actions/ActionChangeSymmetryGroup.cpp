#include "ActionChangeSymmetryGroup.h"

void ActionChangeSymmetryGroup::apply(Drawing& drawing) {
	drawing.set_symmetry_group(new_group);
}

void ActionChangeSymmetryGroup::undo(Drawing& drawing) {
	drawing.set_symmetry_group(old_group);
}
