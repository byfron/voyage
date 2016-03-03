#pragma once

#include <string>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <SDL.h>
#include <memory>

class TextureAtlas {

public:
	
	TextureAtlas(std::string file) { loadTexture(file); }
	~TextureAtlas() {}
	int loadTexture(std::string file);
	void getTileTexCoordinates(const int tileId, float & tX, float & tY, float & tX2, float & tY2);
	int getTextureMode() { return _mode;}
	inline GLuint getTextureId() { return _textureId; }
	inline int getSpriteW() { return _spriteWidth; }
	inline int getSpriteH() { return _spriteHeight; }
	
	
	typedef std::shared_ptr<TextureAtlas> Ptr;
private:

	GLuint _textureId;	
	SDL_Surface* _surface;
	int _textureWidth;
	int _textureHeight;
	int _spriteWidth;
	int _spriteHeight;
	int _mode;
};
