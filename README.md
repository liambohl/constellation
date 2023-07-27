# Constellation is a Windows desktop application for drawing tilings.

## Dependencies
Constellation relies on the following dependencies, bundled with the executable:

1. *Windows GDI+* (graphics library)
	- GDI+ is used for drawing elements to the screen.
2. *JSON for Modern C++*
	- This JSON library can be found online
		- source code: https://github.com/nlohmann/json
		- documentation: https://json.nlohmann.me/
	- Constellation drawings are stored as .cst files using JSON. *JSON for Modern C++* is used for transforming between Constellation drawings and .cst documents.
	- This library is very user-friendly and I highly recommend it.

## Coordinate spaces
GDI+ uses three coordinate spaces: world, page, and device.
Constellation is only concerned with world space and page space.

World space uses units of millimeters, as measured by the rulers on the top and left edges of the application.
The origin of world space can be thought of as the "center" of the drawing.
Each element of a Constellation drawing is defined relative to world space.

Page space uses units of device pixels, measured from the top-left of the client area of the application window.

In both world space and device space, X-coordinates increase as you move to the right, and Y-coordinates increase as you move *downward*.

The transformation between world space and page space changes as the user pans and zooms.
This transformation is maintained by the Canvas object and passed to the Gdiplus\::Graphics object.
When elements are drawn to the screen, they can pass their coordinates to the Gdiplus\::Graphics object in world space,
and Gdiplus applies the transformations to page and then device space.

- For more information, see https://learn.microsoft.com/en-us/windows/win32/gdiplus/-gdiplus-types-of-coordinate-systems-about
