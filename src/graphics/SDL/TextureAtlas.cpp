#include "TextureAtlas.hpp"
#include <SDL_image.h>
#include <SDL.h>
#include <iostream>
#include <GL/glut.h>

int TextureAtlas::loadTexture(std::string file) {

	_surface = IMG_Load(GRAPHICS_DIR "/Spacefox.png");

	if (_surface == NULL) std::cout << "Error loading image:" << IMG_GetError() << std::endl;

	_textureWidth = _surface->w;
	_textureHeight = _surface->h;
	_spriteWidth = 24;
	_spriteHeight = 24;

	//create texture to hold the atlas
	_textureId = 0;
	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	_mode = GL_RGB;
 
	if(_surface->format->BytesPerPixel == 4) {
		_mode = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, _mode, _surface->w, _surface->h, 0, _mode, GL_UNSIGNED_BYTE, _surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

}

void TextureAtlas::getTileTexCoordinates(const int tileId, float & tX, float & tY, float & tX2, float & tY2) {

	//we should keep a map with the id graphic correspondences
	int x = tileId*_spriteWidth;
	int y = 0;
	
	//tmp
	tX = x / (float)_textureWidth;
	tY = y / (float)_textureHeight;
	tX2 = ((x + _spriteWidth) / (float)_textureWidth);
	tY2 = ((y + _spriteHeight) / (float)_textureHeight);
	
	
}
