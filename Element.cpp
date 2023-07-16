#include "Element.h"

namespace Constellation {

	Element::Element(json element_json) {
		id = element_json["id"];
	}

	std::shared_ptr<Element> Element::get_element(ULONG id) {
		if (this->id == id)
			return std::shared_ptr<Element>(this);
		return nullptr;
	}

}