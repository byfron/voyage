#pragma once
#include <vector>
#include <SDL_opengl.h>
#include <graphics/Gui/imgui.h>
#include <clientEngine/ClientTileMap.hpp>
#include <memory>

class TextureAtlas;
typedef std::shared_ptr<TextureAtlas> TextureAtlasPtr;

class TileMapLayer {
public:

	TileMapLayer() {};
	void init();
	void loadTextureAtlas();
	void createTileMapGLTexture();
	void renderTile(int x, int y, int tileId);
//	void renderTile(int x, int y, GLfloat r, GLfloat g, GLfloat b, int tileId);
	void renderTile(int x, int y, GLfloat r, GLfloat g, GLfloat b);
	void render();
	void updateTileMap(ClientTileMap::Ptr tilemap);
		
	void setLayerSize(int w, int h) {_layerW = w; _layerH = h; }
	void setScreenSize(int w, int h) {_screenW = w; _screenH = h; }
	
	ImTextureID getTextureId() { return _tex_id; }
	TextureAtlasPtr getAtlas() { return _texAtlas; }
	
	typedef std::shared_ptr<TileMapLayer> Ptr;	
	
private:

//	std::vector<Tile> _tiles;
	ImTextureID _tex_id;
	GLuint _fb;
	GLuint _renderedTex; //shall this go in the tilemap?
	TextureAtlasPtr _texAtlas;
	GLuint _frameBufferId;
	int _layerH;
	int _layerW;
	int _screenH;
	int _screenW;

	GLfloat _debug_colors[200][200];
};
 
