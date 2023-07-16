#pragma once

#include <memory>
#include <vector>

#include "Element.h"

namespace Constellation {

	class Group :
		public Element
	{
	public:
		Group(ULONG id) : Element(id) {}
		Group(json group_json);

		void draw(Canvas& canvas) override;

		json to_json() override;

		// Sometimes we need to get an element by id because we may not have a pointer to that elemet,
		// as it may have been destroyed and recreated by undoing and redoing.
		std::shared_ptr<Element> get_element(ULONG id) override;
		void add_element(std::shared_ptr<Element> element);
		bool remove_element(ULONG id);

	private:
		std::vector<std::shared_ptr<Element>> elements;
	};

}
