#pragma once

#include <graphics/SDL/ImguiInterface.hpp>
#include <graphics/Gui/imgui.h>
#include <graphics/SDL/TileMap.hpp>
#include <memory>

class Gui {
public:
	Gui();
	void init();
	void display();
	void displayConsole();
	void displayMain();
	void displayMap(TileMap::Ptr);
	void clear();

	int getWindowWidth() { return _window_width; }
	int getWindowHeight() { return _window_height; }
	typedef std::shared_ptr<Gui> Ptr;


	void debug_load_im();

private:

	ImGuiWindowFlags _console_window_flags;
	ImGuiWindowFlags _map_window_flags;
	ImVec4 _clear_color;
	int _window_width;
	int _window_height;
	bool _fullscreen;
	float _window_alpha;

	//debug
	GLuint _sdlOverlayTexId;
	SDL_Surface *_surface;
};

