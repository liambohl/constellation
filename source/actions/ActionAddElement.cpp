#include "ActionAddElement.h"


void ActionAddElement::apply(Drawing& drawing) {
	drawing.add_element(element);
}

void ActionAddElement::undo(Drawing& drawing) {
	drawing.remove_element(element);
}
