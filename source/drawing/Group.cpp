#include "Group.h"

#include "Path.h"


Group::Group(json group_json) {
	for (json element_json : group_json["elements"]) {
		std::string type = element_json["type"];
		if (type == "Group") {
			std::shared_ptr<Element> e = std::make_shared<Group>(element_json);
				elements.push_back(e);
		}
		else if (type == "Path") {
			std::shared_ptr<Element> e = std::make_shared<Path>(element_json);
			elements.push_back(e);
		}
		else
			throw std::exception("Unrecognized element type");
	}
}

// Only non-group elements should apply transforms, to avoid drawing extra copies of elements.
void Group::draw(Gdiplus::Graphics* graphics, std::vector<std::shared_ptr<Gdiplus::Matrix>>& transforms) {
	for (std::shared_ptr<Element> element : elements) {
		element->draw(graphics, transforms);
	}
}

void Group::draw_original(Gdiplus::Graphics* graphics) {
	for (std::shared_ptr<Element> element : elements) {
		element->draw_original(graphics);
	}
}

std::optional<Gdiplus::RectF> Group::get_bounding_box() {
	std::optional<Gdiplus::RectF> bounds;
	std::optional<Gdiplus::RectF> element_bounds;

	for (std::shared_ptr<Element> el : elements) {
		element_bounds = el->get_bounding_box();
		if (!element_bounds)	// Element is empty
			continue;
		if (!bounds)			// Element is first nonempty element
			bounds = element_bounds;
		else {					// Grow bounds to fit element
			float left = min(bounds->GetLeft(), element_bounds->GetLeft());
			float top = min(bounds->GetTop(), element_bounds->GetTop());
			float right = max(bounds->GetRight(), element_bounds->GetRight());
			float bottom = max(bounds->GetBottom(), element_bounds->GetBottom());

			bounds->X = left;
			bounds->Y = top;
			bounds->Width = right - left;
			bounds->Height = bottom - top;
		}
	}

	return bounds;
}

bool Group::try_select(const Gdiplus::PointF& cursor_pos) {
	for (auto element : elements) {
		if (element->try_select(cursor_pos))
			return true;
	}
	return false;
}

std::shared_ptr<Element> Group::select_in_group(const Gdiplus::PointF& cursor_pos) {
	for (auto element : elements) {
		if (element->try_select(cursor_pos))
			return element;
	}
	return nullptr;
}

json Group::to_json() {
	json output = {
		{"type", "Group"},
		{"elements", {}}
	};

	// Populate elements
	for (std::shared_ptr<Element> el : elements) {
		output["elements"].push_back(el->to_json());
	}

	return output;
}

void Group::add_element(std::shared_ptr<Element> element) {
	elements.push_back(element);
}

bool Group::remove_element(std::shared_ptr<Element> element) {
	for (auto i = elements.begin(); i != elements.end(); ++i) {
		if (*i == element) {
			elements.erase(i);
			return true;
		}
	}
	return false;
}
