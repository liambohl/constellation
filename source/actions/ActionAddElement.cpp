#include "ActionAddElement.h"

void ActionAddElement::apply(Drawing& drawing) {
	drawing.elements.add_element(element);
}

void ActionAddElement::undo(Drawing& drawing) {
	drawing.elements.remove_element(element);
}
