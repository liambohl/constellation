#include "ActionRemoveElements.h"


void ActionRemoveElements::apply(Drawing& drawing) {
	drawing.remove_elements(elements);
}

void ActionRemoveElements::undo(Drawing& drawing) {
	drawing.add_elements(elements);
}
