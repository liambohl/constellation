#include "Drawing.h"
#include "json_converters.h"
#include "core/Logger.h"
#include "symmetry/SymmetryGroupFactory.h"

Drawing::Drawing() {
	background = Gdiplus::Color(255, 80, 80, 80);
	symmetry_group = SymmetryGroupFactory::get_instance().trivial();
}

Drawing::Drawing(json drawing_json) :
	background(color_from_json(drawing_json["background"])),
	symmetry_group(SymmetryGroupFactory::get_instance().from_json(drawing_json["symmetry_group"])),
	elements(drawing_json["elements"])
{}

// Draw this drawing to the window
void Drawing::draw(Gdiplus::Graphics* graphics) {
	graphics->Clear(background);
	std::vector<std::shared_ptr<Gdiplus::Matrix>> transforms = symmetry_group->get_transforms();
	elements.draw(graphics, transforms);
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
		{"background", color_to_json(background)},
		{"symmetry_group", symmetry_group->to_json()},
		{"elements", elements.to_json()}
	};
}