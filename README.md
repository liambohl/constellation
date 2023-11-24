# Constellation is a Windows desktop application for drawing tilings.

## Getting Started

### Step 0: Install Constellation
Constellation is currently only supported on Windows 11. This is
an oversight, and I plan to support earlier versions of Windows
in future releases. Visit [the releases page](https://github.com/liambohl/constellation/releases)
to find the latest release, where you can find both an installer
and a portable version of Constellation under Assets.
If you download and run the installer, Windows Defender
SmartScreen will discourage you from installing Constellation
because I haven't signed the installer. This is because
code-signing certificates are expensive, but I may fix this
in the future. Ignore the warning from SmartScreen.

### Step 1: Draw a path
A Bézier curve is a smooth curve defined by four control points.
The curve starts at the first and ends at the last control point,
while the other two control points define tangent lines to the path
at the first and last control point. If the first and second control
point are equal, and the third and fourth control point are equal,
the curve is a line segment.

In Constellation, you can draw *paths*, which consist of one or more
Bézier curves joined end-to-end.
To Draw a path, press 'p' or select Draw -> New Path.
Click to set the first control point, drag, and release to set the
second control point. Then click to set the *fourth* control point,
drag, and release to set the third and fifth control points,
indicated by the magenta tangent line. Continue adding to your path
by clicking and dragging. Right click to finish the path.

### Step 2: Edit a path
Constellation does not yet support directly editing the control
points of a path, changing its line thickness and color,
or filling a path with a color. However, with the select tool
(Draw -> Select, 's', or 'esc'), you can move or resize a path.
With a path selected, you can click it to switch to rotate mode.
Try holding control, holding shift, or moving the rotation center
to change how the rotate and resize functions work.

### Step 3: Choose a symmetry group
The most important feature of Constellation is that it can apply
symmetries to your drawing as you draw and edit. These symmetries
allow you to tile the plane with copies of your drawn elements.
The supported symmetry groups so far are the 17 [wallpaper groups](https://en.wikipedia.org/wiki/Wallpaper_group).
To choose a symmetry group, select Symmetry -> Choose Symmetry Group.
The illustrations show how the letter 'F' would be copied and
transformed by each symmetry group.

When a wallpaper group is selected, you will see some blue
and orange guides. The blue region is the recommended drawing area.
Drawing outside this area can create some weird tilings.
The orange guides show the fundamental domains
of the current symmetry group. As indicated by the jigsaw puzzle
edges of these guides, there are many ways they could be drawn.
Don't let these regions limit your creativity. In fact,
use the View menu to turn them off if you wish.

### Step 4: Edit a symmetry group
If you have selected a non-trivial symmetry group, you can edit it
by selecting Symmetry -> Edit Symmetry Parameters.
Drag the arrows to change how your drawn elements are transformed.

### Step 5: Save your drawing
You can save your drawing as a .cst file and use Constellation
to open .cst files. Future major updates of Constellation, such as
from 0.5 to 1.0, but not 0.5 to 0.6, may break compatibility
with .cst files.

## How Constellation Works

### Dependencies
Constellation relies on the following dependencies, bundled with the executable:

1. *Windows GDI+* (graphics library)
	- GDI+ is used for drawing elements to the screen.
2. *JSON for Modern C++*
	- This JSON library can be found online
		- source code: https://github.com/nlohmann/json
		- documentation: https://json.nlohmann.me/
	- Constellation drawings are stored as .cst files using JSON. *JSON for Modern C++* is used for
	transforming between Constellation drawings and .cst documents.
	- This library is very user-friendly and I highly recommend it.

### Coordinate spaces
GDI+ uses three coordinate spaces: world, page, and device.
Constellation is only concerned with world space and page space.

World space uses units of millimeters, as measured by the rulers on the top and left edges of the application.
The origin of world space can be thought of as the "center" of the drawing.
Each element of a Constellation drawing is defined relative to world space.

Page space uses units of device pixels, measured from the top-left of the client area of the application window.

In both world space and device space, X-coordinates increase as you move to the right,
and Y-coordinates increase as you move *downward*.

The transformation between world space and page space changes as the user pans and zooms.
This transformation is maintained by the Canvas object and passed to the Gdiplus\::Graphics object.
When elements are drawn to the screen, they can pass their coordinates to the Gdiplus\::Graphics object in world space,
and Gdiplus applies the transformations to page and then device space.

- For more information, see https://learn.microsoft.com/en-us/windows/win32/gdiplus/-gdiplus-types-of-coordinate-systems-about
