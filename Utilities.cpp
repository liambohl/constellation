#include "utilities.h"


bool control_key_down() {
	return GetKeyState(VK_CONTROL) < 0;
}