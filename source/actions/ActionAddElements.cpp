#include "ActionAddElements.h"


void ActionAddElements::apply(Drawing& drawing) {
	drawing.add_elements(elements);
}

void ActionAddElements::undo(Drawing& drawing) {
	drawing.remove_elements(elements);
}
