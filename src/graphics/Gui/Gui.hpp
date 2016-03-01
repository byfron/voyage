#pragma once

#include <graphics/Gui/imgui.h>
#include <memory>

class Gui {
public:
	void init();
	void display();
	void clear();

	typedef std::shared_ptr<Gui> Ptr;

private:

	ImVec4 _clear_color;
	
};
