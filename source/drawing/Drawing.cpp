#include "Drawing.h"
#include "json_converters.h"
#include "core/Logger.h"
#include "symmetry/SymmetryGroupFactory.h"


Drawing::Drawing() {
	symmetry_group = SymmetryGroupFactory::get_instance().trivial();
}

Drawing::Drawing(json drawing_json) :
	symmetry_group(SymmetryGroupFactory::get_instance().from_json(drawing_json["symmetry_group"])),
	elements(drawing_json["elements"])
{}

// Draw this drawing to the given buffer
void Drawing::draw(Gdiplus::Graphics* graphics) {
	std::vector<std::shared_ptr<Gdiplus::Matrix>> transforms = symmetry_group->get_transforms();
	elements.draw(graphics, transforms);
}

// Draw only the original (user-drawn, not transformed copy) elements to the given buffer
void Drawing::draw_original(Gdiplus::Graphics* graphics) {
	elements.draw_original(graphics);
}

Gdiplus::RectF* Drawing::get_bounding_box() {
	Gdiplus::RectF* bounding_box = nullptr;
	elements.get_bounding_box(&bounding_box);
	*Logger::get_instance() << "bounding box (x, y, width, height) = ("
		<< bounding_box->X << ", " << bounding_box->Y << ", "
		<< bounding_box->Width << ", " << bounding_box->Height
		<< ")" << std::endl;
	return bounding_box;
}

json Drawing::to_json() {
	return {
		{"symmetry_group", symmetry_group->to_json()},
		{"elements", elements.to_json()}
	};
}
