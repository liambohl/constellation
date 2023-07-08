#include "Group.h"

namespace Constellation {

	Group::~Group() {
		for (int i = 0; i < elements.size(); ++i) {
			delete elements[i];
		}
	}
	
	void Group::draw(Canvas& canvas) {
		for (Element* element : elements) {
			element->draw(canvas);
		}
	}

	Element* Group::get_element(ULONG id) {
		if (this->id == id)
			return this;

		for (Element* element : elements) {
			Element* found_element = element->get_element(id);
			if (found_element != nullptr)
				return found_element;
		}
		return nullptr;
	}

	void Group::add_element(Element* element) {
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
