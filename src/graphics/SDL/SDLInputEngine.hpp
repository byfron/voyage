#pragma once 
#include <SDL.h>

class SDLInputEngine {
public:
	static bool g_MousePressed[3];
	static float g_MouseWheel;
	
	bool init(SDL_Window *window);
	bool processEvent(SDL_Event* event);	
};
