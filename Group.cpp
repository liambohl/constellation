#include "Group.h"
#include "Path.h"

namespace Constellation {

	Group::Group(json group_json) : Element(group_json) {
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
	
	void Group::draw(Canvas& canvas) {
		for (std::shared_ptr<Element> element : elements) {
			element->draw(canvas);
		}
	}

	json Group::to_json() {
		json output = {
			{"type", "Group"},
			{"id", id},
			{"elements", {}}
		};

		// Populate elements
		for (std::shared_ptr<Element> el : elements) {
			output["elements"].push_back(el->to_json());
		}

		return output;
	}

	std::shared_ptr<Element> Group::get_element(ULONG id) {
		if (this->id == id)
			return std::shared_ptr<Element>(this);

		for (std::shared_ptr<Element> element : elements) {
			std::shared_ptr<Element> found_element = element->get_element(id);
			if (found_element != nullptr)
				return found_element;
		}
		return nullptr;
	}

	void Group::add_element(std::shared_ptr<Element> element) {
		elements.push_back(element);
	}

	bool Group::remove_element(ULONG id) {
		for (auto element = elements.begin(); element != elements.end(); ++element) {
			if ((*element)->id == id) {
				elements.erase(element);
				return true;
			}
		}
		return false;
	}

}
