#include "ActionTransformElements.h"


ActionTransformElements::~ActionTransformElements() {
    delete transform;
}

void ActionTransformElements::apply(Drawing& drawing) {
    for (auto& element : elements) {
        element->transform(transform);
    }
}

void ActionTransformElements::undo(Drawing& drawing) {
    transform->Invert();
    for (auto& element : elements) {
        element->transform(transform);
    }
    transform->Invert();
}
