#include "utilities.h"


bool control_key_down() {
	return GetKeyState(VK_CONTROL) < 0;
}

bool shift_key_down() {
	return GetKeyState(VK_SHIFT) < 0;
}