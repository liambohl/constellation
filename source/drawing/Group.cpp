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

void Group::get_bounding_box(Gdiplus::RectF** bounding_box) {
	for (std::shared_ptr<Element> el : elements) {
		el->get_bounding_box(bounding_box);
	}
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

// Remove an element if it is a direct child of this group
bool Group::remove_element(std::shared_ptr<Element> element) {
	for (auto i = elements.begin(); i != elements.end(); ++i) {
		if (*i == element) {
			elements.erase(i);
			return true;
		}
	}
	return false;
}
