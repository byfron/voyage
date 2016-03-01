#include "Gui.hpp"
#include <SDL_opengl.h>
#include <graphics/SDL/ImguiInterface.hpp>

void Gui::init() {
	_clear_color = ImColor(114, 144, 154);	
}

void Gui::display() {

	bool opened = true;
	float bg_alpha = -0.01f; // <0: default
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	// ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
	// if (!ImGui::Begin("voyage_console", &opened, ImVec2(550,680), bg_alpha, window_flags))
	// {
	// 	// Early out if the window is collapsed, as an optimization.
	// 	ImGui::End();
	// 	return;
	// }
	
	static float f = 0.0f;
	ImGui::Text("Hello, world!");
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	ImGui::ColorEdit3("clear color", (float*)&_clear_color);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	
}

void Gui::clear() {
	glClearColor(_clear_color.x, _clear_color.y, _clear_color.z, _clear_color.w);         
        glClear(GL_COLOR_BUFFER_BIT);
}
