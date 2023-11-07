#pragma once

#include "Tool.h"

#include <vector>

#include "drawing/Group.h"


class ToolSelect :
    public Tool
{
public:
    ToolSelect(Drawing& drawing, Defaults& defaults) : Tool(drawing, defaults) { }

    Action* handle_mouse_event(UINT message, Gdiplus::PointF cursor_pos, int key_state, float scale) override;

    void draw(
		Gdiplus::Graphics* graphics,
		Gdiplus::PointF cursor_pos,
		std::vector<std::shared_ptr<Gdiplus::Matrix>>& transforms,
		float scale
	) override;

	void update() override;

	void select_all() override;
	virtual void select_elements(std::vector<std::shared_ptr<Element>> elements) override;

	std::vector<std::shared_ptr<Element>> get_selection() override { return selection; }

	Action* handle_delete() override;

	boolean handle_escape() override;

private:
	enum Mode {
		RESIZE,	// dragging handles resizes selected elements
		ROTATE	// dragging handles rotates selected elements around center point
	};

	enum State {
		IDLE,
		DRAGGING_HANDLE,	// Using handles to rotate or resize selection
		DRAGGING_SELECTION,	// Moving selection
		SELECTING_AREA		// Creating selection rectangle
	};

	void add_or_remove_element(std::shared_ptr<Element> element);	// If the selection contains the element, remove it. Otherwise, add it.

	void deselect_all();	// Remove everything from selection and return to resize mode

	void update_bounds();
	void update_rotation_center();

	void set_click_offset();

	// Calculate the transform during a rotate or resize operation. Temporarily apply it to the selection.
	void rotate_transform(Gdiplus::PointF& cursor_pos, Gdiplus::Matrix* transform, bool shift_pressed, bool control_pressed);
	void resize_transform(Gdiplus::PointF& cursor_pos, Gdiplus::Matrix* transform, bool shift_pressed, bool control_pressed);

	HandleMap get_handles(float scale) override;

	Mode mode = RESIZE;
	State state = IDLE;

	// This is true only while the left mouse button is pressed upon creating a new selection.
	// When true, releasing the LMB does not switch between resize mode and rotate mode.
	// This way, we don't switch modes every time we select something new.
	bool new_selection = false;

	std::optional<std::string> active_handle;

	std::vector<std::shared_ptr<Element>> selection;

	Gdiplus::PointF click_position;		// Cursor position when we click (start dragging a handle or the selection or start selecting an area)
	// When we select a resize handle, click_offset is equal to click_position
	// minus the position of the bounding box corner or edge midpoint corresponding to the resize handle.
	Gdiplus::PointF click_offset;

	Gdiplus::PointF rotation_center;
	
	std::optional<Gdiplus::RectF> bounds;			// Bounding box of selection, if selection is nonempty
	std::optional<Gdiplus::RectF> initial_bounds;	// Bounding box of selection before current resize operation

	static const float SELECTION_MARGIN;
};
