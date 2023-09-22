#include "Value.h"


std::shared_ptr<Value> operator-(std::shared_ptr<Value> e) { return std::make_shared<Value>(-e->value); }
