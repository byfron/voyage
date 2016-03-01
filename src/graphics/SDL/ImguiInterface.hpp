#pragma once
#include <graphics/Gui/imgui.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "SDLInputEngine.hpp"

void ImGuiRenderDrawLists(ImDrawData* draw_data);
const char* ImGuiGetClipboardText();
void ImGuiSetClipboardText(const char* text);
