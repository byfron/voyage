#include "TileMap.hpp"
#include "TextureAtlas.hpp"
#include <memory>

void TileMap::init() {
	
	//create frame buffer
	_frameBufferId = 0;
//	glGenFramebuffers(1, &_frameBufferId);

}

void TileMap::loadTextureAtlas() {
	//create texture of the tilemap
	_texAtlas = std::make_shared<TextureAtlas>("/mayday.png");

	//associate imgui texture with opengl texture
	_tex_id = (void *)(intptr_t) _texAtlas->getGLTexId();
}

void TileMap::render() {
		
//	glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferId);	
	
	//render sprites
//	_texAtlas->renderSprite(1, 0, 0);
//	ImGui::Image(_tex_id, ImVec2(256, 256), ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255), ImColor(255,255,255,128));

	//unbind frame buffer
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
