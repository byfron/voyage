#include "SDLInputEngine.hpp"
#include <graphics/Gui/imgui.h>

// Data
extern bool g_MousePressed[3];
extern float g_MouseWheel;

bool SDLInputEngine::g_MousePressed[3] = {false, false, false};
float SDLInputEngine::g_MouseWheel = 0;

bool SDLInputEngine::init(SDL_Window *window) {
	
	ImGuiIO& io = ImGui::GetIO();

	io.KeyMap[ImGuiKey_Tab] = SDLK_TAB;     // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
	io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
	io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
	io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
	io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
	io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
	io.KeyMap[ImGuiKey_Delete] = SDLK_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = SDLK_BACKSPACE;
	io.KeyMap[ImGuiKey_Enter] = SDLK_RETURN;
	io.KeyMap[ImGuiKey_Escape] = SDLK_ESCAPE;
	io.KeyMap[ImGuiKey_A] = SDLK_a;
	io.KeyMap[ImGuiKey_C] = SDLK_c;
	io.KeyMap[ImGuiKey_V] = SDLK_v;
	io.KeyMap[ImGuiKey_X] = SDLK_x;
	io.KeyMap[ImGuiKey_Y] = SDLK_y;
	io.KeyMap[ImGuiKey_Z] = SDLK_z;
	
#ifdef _WIN32
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(window, &wmInfo);
	io.ImeWindowHandle = wmInfo.info.win.window;
#endif

	return true;
}

bool SDLInputEngine::processEvent(SDL_Event* event) {

	ImGuiIO& io = ImGui::GetIO();
	switch (event->type)
	{
	case SDL_MOUSEWHEEL:
        {
		if (event->wheel.y > 0)
			g_MouseWheel = 1;
		if (event->wheel.y < 0)
			g_MouseWheel = -1;
		return true;
        }
	case SDL_MOUSEBUTTONDOWN:
        {
		if (event->button.button == SDL_BUTTON_LEFT) g_MousePressed[0] = true;
		if (event->button.button == SDL_BUTTON_RIGHT) g_MousePressed[1] = true;
		if (event->button.button == SDL_BUTTON_MIDDLE) g_MousePressed[2] = true;
		return true;
        }
	case SDL_TEXTINPUT:
        {
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharactersUTF8(event->text.text);
		return true;
        }
	case SDL_KEYDOWN:
	case SDL_KEYUP:
        {
		int key = event->key.keysym.sym & ~SDLK_SCANCODE_MASK;
		io.KeysDown[key] = (event->type == SDL_KEYDOWN);
		io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
		io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
		io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
		return true;
        }
	}
	
}
