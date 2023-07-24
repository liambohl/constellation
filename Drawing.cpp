#include "Drawing.h"
#include "Logger.h"

namespace Constellation {

	Drawing::Drawing(json drawing_json) :
		next_id(drawing_json["next_id"]),
		defaults(drawing_json["defaults"]),
		elements(drawing_json["elements"])
	{}

	// Draw this drawing to the window
	void Drawing::draw(Canvas& canvas) {
		canvas.graphics->Clear(defaults.background);

		elements.draw(canvas);
	}

	json Drawing::to_json() {
		return {
			{"next_id", next_id},
			{"defaults", defaults.to_json()},
			{"elements", elements.to_json()}
		};
	}

}
