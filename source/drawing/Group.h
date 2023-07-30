#pragma once

#include <memory>
#include <vector>

#include "Element.h"
#include "framework.h"

class Group :
	public Element
{
public:
	Group() {}
	Group(json group_json);

	void draw(Gdiplus::Graphics* graphics) override;
	void get_bounding_box(Gdiplus::RectF** bounding_box);

	json to_json() override;

	void add_element(std::shared_ptr<Element> element);
	bool remove_element(std::shared_ptr<Element> element);

private:
	std::vector<std::shared_ptr<Element>> elements;
};
