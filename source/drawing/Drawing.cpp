#include "Drawing.h"

#include "json_converters.h"
#include "core/Logger.h"
#include "core/resource.h"
#include "symmetry/SymmetryGroupFactory.h"


Drawing::Drawing(HWND hWnd) : hWnd(hWnd) {
    set_symmetry_group(SymmetryGroupFactory::get_instance().trivial());
}

Drawing::Drawing(HWND hWnd, json drawing_json) :
    hWnd(hWnd),
    elements(drawing_json["elements"])
{
    set_symmetry_group(SymmetryGroupFactory::get_instance().from_json(drawing_json["symmetry_group"]));
}

// Draw this drawing to the given buffer
void Drawing::draw(Gdiplus::Graphics* graphics) {
    std::vector<std::shared_ptr<Gdiplus::Matrix>> transforms = symmetry_group->get_transforms();
    elements.draw(graphics, transforms);
}

// Draw only the original (user-drawn, not transformed copy) elements to the given buffer
void Drawing::draw_original(Gdiplus::Graphics* graphics) {
    elements.draw_original(graphics);
}

void Drawing::set_symmetry_group(std::shared_ptr<SymmetryGroup> group) {
    symmetry_group = group;
    // Enable "Edit Symmetry Parameters" tool only if this is a wallpaper group
    if (std::dynamic_pointer_cast<WallpaperGroup>(symmetry_group))
        EnableMenuItem(GetMenu(hWnd), ID_SYMMETRY_EDITSYMMETRYPARAMETERS, MF_ENABLED);
    else
        EnableMenuItem(GetMenu(hWnd), ID_SYMMETRY_EDITSYMMETRYPARAMETERS, MF_GRAYED);
}

json Drawing::to_json() {
    return {
        {"symmetry_group", symmetry_group->to_json()},
        {"elements", elements.to_json()}
    };
}
