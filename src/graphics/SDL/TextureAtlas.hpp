#pragma once

#include <string>
#include <SDL_opengl.h>
#include <SDL.h>
#include <memory>

class TextureAtlas {

public:
	
	TextureAtlas(std::string file) { loadTexture(file); }
	~TextureAtlas() { delete _data;}
	int loadTexture(std::string file);
	void renderSprite(int spriteId, int x, int y);	
	GLuint getGLTexId() { return _textureId; }
	void createSDLRenderer(SDL_Window *w);
	typedef std::shared_ptr<TextureAtlas> Ptr;
private:

	GLuint _textureId;
	GLuint _spriteId;
	SDL_Surface* _surface;
	int _textureWidth;
	int _textureHeight;
	int _spriteWidth;
	int _spriteHeight;

	GLubyte *_data;
	SDL_Renderer * _renderer;	
};
