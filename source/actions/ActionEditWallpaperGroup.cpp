#include "ActionEditWallpaperGroup.h"

#include "drawing/symmetry/WallpaperGroup.h"


void ActionEditWallpaperGroup::apply(Drawing& drawing) {
	auto group = std::static_pointer_cast<WallpaperGroup>(drawing.get_symmetry_group());
	if (vector == EDIT_V1)
		group->set_v1(new_x, new_y);
	else
		group->set_v2(new_x, new_y);
}

void ActionEditWallpaperGroup::undo(Drawing& drawing) {
	auto group = std::static_pointer_cast<WallpaperGroup>(drawing.get_symmetry_group());
	if (vector == EDIT_V1)
		group->set_v1(old_x, old_y);
	else
		group->set_v2(old_x, old_y);
}
