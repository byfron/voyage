#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "SDLInputEngine.hpp"
#include <clientEngine/GameClient.hpp>
#include <graphics/Gui/imgui.h>
#include <graphics/Gui/Gui.hpp>

class SDLGraphicsEngine {

public:

	static GLuint g_FontTexture;
	static double g_Time;
	
	SDLGraphicsEngine() {}
	~SDLGraphicsEngine() {}
	SDLGraphicsEngine(GameClient::Ptr gc) { _gameClient = gc; }
	void init();
	void run();
	void stop();
	void newFrame();
	bool createDeviceObjects();

	typedef std::shared_ptr<SDLGraphicsEngine> Ptr;

private:

	SDLInputEngine _input;
	GameClient::Ptr _gameClient;
	Gui::Ptr _gui;
	TileMap::Ptr _tilemap;
	
	SDL_Window *_window;
	SDL_GLContext _glcontext;

	//DEBUG	
	bool _show_test_window;
	bool _show_another_window;	
	
};

