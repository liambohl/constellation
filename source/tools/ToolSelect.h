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
	Gdiplus::PointF last_drag_position;	// Cursor position last time we handled a WM_MOUSEMOVE event
	
	std::optional<Gdiplus::RectF> bounds;

	static const float SELECTION_MARGIN;
};
