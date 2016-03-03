#include "TextureAtlas.hpp"
//#include <GL/glew.h>
#include <SDL_image.h>
#include <SDL.h>
#include <iostream>

void TextureAtlas::createSDLRenderer(SDL_Window *w) {
	_renderer = SDL_CreateRenderer(w, -1,
				       SDL_RENDERER_ACCELERATED |
				       SDL_RENDERER_TARGETTEXTURE);
	
}

int TextureAtlas::loadTexture(std::string file) {

	_surface = IMG_Load(GRAPHICS_DIR "/mayday.png");

	if (_surface == NULL) std::cout << "Error loading image:" << IMG_GetError() << std::endl;


	_textureWidth = _surface->w;
	_textureHeight = _surface->h;
	_spriteWidth = 16;
	_spriteHeight = 16;


/*	SDL_Texture *bmpTex = SDL_CreateTextureFromSurface(_renderer, _surface);
	//Use SDL to render to texture. We can get create the renderer from the window
	//Now render to the texture
	SDL_Texture *texTarget = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888,
						   SDL_TEXTUREACCESS_TARGET, _surface->w,
						   _surface->h);
	
	SDL_SetRenderTarget(_renderer, texTarget);
	SDL_RenderClear(_renderer);
	SDL_RenderCopy(_renderer, bmpTex, NULL, NULL);

*/

	//create texture
	_textureId = 0;
	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	int Mode = GL_RGB;
 
	if(_surface->format->BytesPerPixel == 4) {
		Mode = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, Mode, _surface->w, _surface->h, 0, Mode, GL_UNSIGNED_BYTE, _surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	

}

void TextureAtlas::renderSprite(int spriteId, int X, int Y) {

/*	glGenTextures(1, &_spriteId);
	glBindTexture(GL_TEXTURE_2D, _spriteId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, _surface->w);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16,  16, 0, 
		     GL_RGBA, GL_UNSIGNED_BYTE, _data + 4*(Y*_surface->w+X));
*/
    // clean up



	
	//  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	/*
	float SrcX = 0;
	float SrcY = 0;
	
// Bind the texture to which subsequent calls refer
	//set rendering target to frame_buffer
//	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _textureId, 0);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	
	float tX = (SrcX / (float)_textureWidth);
	float tY = (SrcY / (float)_textureHeight);
	
	float tX2 = ((SrcX + _spriteWidth) / (float)_textureWidth);
	float tY2 = ((SrcY + _spriteHeight) / (float)_textureHeight);
	
//This combination is matched with the current Ortho view
//Don't change the Ortho view, otherwise textures will be flipped
//or rotated
	glBegin(GL_QUADS);
	
	glTexCoord2f(tX, tY);
	glVertex3f(X, Y, 0);
	
	glTexCoord2f(tX2, tY);
	glVertex3f(X + _spriteWidth, Y, 0);
	
	glTexCoord2f(tX2, tY2);
	glVertex3f(X + _spriteWidth, Y + _spriteHeight, 0);
	
	glTexCoord2f(tX, tY2);
	glVertex3f(X, Y + _spriteHeight, 0);
	
	glEnd();
	*/
	
}
