#pragma once
#include <vector>
#include <SDL_opengl.h>
#include <graphics/Gui/imgui.h>
#include <memory>

class TextureAtlas;
typedef std::shared_ptr<TextureAtlas> TextureAtlasPtr;

class TileMap {
public:

	TileMap() {};
	void init();
	void render();
	void loadTextureAtlas();
	ImTextureID getTextureId() { return _tex_id; }

	TextureAtlasPtr getAtlas() { return _texAtlas; }
	
	typedef std::shared_ptr<TileMap> Ptr;	
	
private:

//	std::vector<Tile> _tiles;
	ImTextureID _tex_id;
	TextureAtlasPtr _texAtlas;
	GLuint _frameBufferId;	
};
 
