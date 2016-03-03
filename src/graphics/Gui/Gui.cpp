#include "Gui.hpp"
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>

Gui::Gui() {}

void Gui::init() {
	_clear_color = ImColor(114, 144, 154);
	_window_width = 1280;
	_window_height = 768;
	_tilelayer_height = _window_height;
	_tilelayer_width = _window_width/2.0;
	_fullscreen = false;
	_console_window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	_map_window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;;
	_window_alpha = -0.01f; // <0: default	
}

void Gui::displayMap(TileMapLayer::Ptr tm) {

	bool opened = true;	
	ImGui::SetNextWindowPos(ImVec2(_tilelayer_width, 0), ImGuiSetCond_FirstUseEver);
	if (!ImGui::Begin("Map", &opened, ImVec2(_tilelayer_width, _window_height),
	 		  _window_alpha, _map_window_flags))
	{
	 	ImGui::End();
	 	return;
	}
		
	tm->render();

	ImGui::Image(tm->getTextureId(), ImVec2(_tilelayer_width-17, _tilelayer_height-17), ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255), ImColor(255,255,255,128));
	ImGui::End();	
}

void Gui::displayMain() {

//	_description_window->display();
	
	bool opened = true;	
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);
	if (!ImGui::Begin("Main", &opened, ImVec2(_window_width/2,_window_height/2),
			  _window_alpha, _console_window_flags))
	{
		ImGui::End();
		return;
	}
	ImGui::Text("This is the main output window");
	ImGui::End();
}

void Gui::displayConsole() {

	bool opened = true;	
	ImGui::SetNextWindowPos(ImVec2(0, _window_height/2), ImGuiSetCond_FirstUseEver);
	if (!ImGui::Begin("Console", &opened, ImVec2(_window_width/2,_window_height/2),
			  _window_alpha, _console_window_flags))
	{
		ImGui::End();
		return;
	}
	
	static float f = 0.0f;
	ImGui::Text("Hello, world!");
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	ImGui::ColorEdit3("clear color", (float*)&_clear_color);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void Gui::clear() {
	glClearColor(_clear_color.x, _clear_color.y, _clear_color.z, _clear_color.w);         
        glClear(GL_COLOR_BUFFER_BIT);
}
