#pragma once

#include <memory>
#include <vector>

#include "Element.h"
#include "core/framework.h"


class Group :
	public Element
{
public:
	Group() {}
	Group(json group_json);

	void draw(Gdiplus::Graphics* graphics, std::vector<std::shared_ptr<Gdiplus::Matrix>>& transforms) override;
	void draw_original(Gdiplus::Graphics* graphics) override;

	std::optional<Gdiplus::RectF> get_bounding_box() override;

	bool try_select(const Gdiplus::PointF& cursor_pos) override;					// Try to select this group
	std::shared_ptr<Element> select_in_group(const Gdiplus::PointF& cursor_pos);	// Try to select an element of this group

	json to_json() override;

	void add_element(std::shared_ptr<Element> element);
	bool remove_element(std::shared_ptr<Element> element);			// Remove an element if it is a direct child of this group

private:
	std::vector<std::shared_ptr<Element>> elements;
};
