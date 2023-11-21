#include "ActionEditWallpaperGroup.h"

#include "drawing/symmetry/WallpaperGroup.h"


void ActionEditWallpaperGroup::apply(Drawing& drawing) {
    auto group = std::static_pointer_cast<WallpaperGroup>(drawing.get_symmetry_group());
    if (vector == EDIT_V1)
        group->set_v1(vector_new);
    else
        group->set_v2(vector_new);
}

void ActionEditWallpaperGroup::undo(Drawing& drawing) {
    auto group = std::static_pointer_cast<WallpaperGroup>(drawing.get_symmetry_group());
    if (vector == EDIT_V1)
        group->set_v1(vector_initial);
    else
        group->set_v2(vector_initial);
}
