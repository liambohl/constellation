#include "Element.h"

namespace Constellation {

	Element* Element::get_element(ULONG id) {
		if (this->id == id)
			return this;
		return nullptr;
	}

}