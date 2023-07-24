#pragma once

#include <memory>
#include <vector>

#include "Element.h"

class Group :
	public Element
{
public:
	Group() {}
	Group(json group_json);

	void draw(Canvas& canvas) override;

	json to_json() override;

	void add_element(std::shared_ptr<Element> element);
	bool remove_element(std::shared_ptr<Element> element);

private:
	std::vector<std::shared_ptr<Element>> elements;
};
