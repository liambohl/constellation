#pragma once

#include<vector>

#include "Element.h"

namespace Constellation {

	class Group :
		public Element
	{
	public:
		Group(ULONG id) : Element(id) {}
		~Group();

		void draw(Canvas& canvas) override;

		// Sometimes we need to get an element by id because we may not have a pointer to that elemet,
		// as it may have been destroyed and recreated by undoing and redoing.
		virtual Element* get_element(ULONG id);
		void add_element(Element* element);
		bool remove_element(ULONG id);

	private:
		std::vector<Element*> elements;
	};

}
