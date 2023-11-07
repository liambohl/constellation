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

std::optional<Gdiplus::RectF> Group::get_bounding_box(bool include_stroke) {
	return Element::get_bounding_box(elements, include_stroke);
}

bool Group::intersects_rectangle(Gdiplus::RectF& rectangle) {
	for (auto& element : elements) {
		if (element->intersects_rectangle(rectangle))
			return true;
	}
	return false;
}

bool Group::try_select(const Gdiplus::PointF& cursor_pos, float margin, float scale) {
	for (auto element : elements) {
		if (element->try_select(cursor_pos, margin, scale))
			return true;
	}
	return false;
}

std::shared_ptr<Element> Group::select_in_group(const Gdiplus::PointF& cursor_pos, float margin, float scale) {
	for (auto element : elements) {
		if (element->try_select(cursor_pos, margin, scale))
			return element;
	}
	return nullptr;
}

void Group::transform(const Gdiplus::Matrix* transform) {
	for (const auto& element : elements)
		element->transform(transform);
}

void Group::transform_temp(const Gdiplus::Matrix* transform) {
	for (const auto& element : elements)
		element->transform_temp(transform);
}

std::shared_ptr<Element> Group::clone() {
	auto clone = std::make_shared<Group>();
	// Copy all elements from this group to clone
	for (const auto& element : elements)
		clone->add_elements({ element->clone() });
	return clone;
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

void Group::add_elements(std::vector<std::shared_ptr<Element>> new_elements) {
	elements.insert(elements.end(), new_elements.begin(), new_elements.end());
}

void Group::remove_elements(std::vector<std::shared_ptr<Element>> elements_to_remove) {
	// Two-step process:
	// std::remove_if reorders elements so the elements to be removed are at the end and returns an iterator to the start of that range to be removed.
	// elements.erase removes that range.
	elements.erase(
		std::remove_if(
			elements.begin(),
			elements.end(),
			// Is this element in elements_to_remove?
			[&elements_to_remove](const std::shared_ptr<Element>& elem) {
				return std::find(elements_to_remove.begin(), elements_to_remove.end(), elem) != elements_to_remove.end();
			}),
		elements.end()
	);
}
