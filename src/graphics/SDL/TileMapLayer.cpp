#include "TextureAtlas.hpp"
#include "TileMapLayer.hpp"
#include <memory>
#include <iostream>

void TileMapLayer::init() {
	
	_frameBufferId = 0;
	_layerW = 500;
	_layerH = 500;
}

void TileMapLayer::loadTextureAtlas() {	
	//create texture of the tilemap
	_texAtlas = std::make_shared<TextureAtlas>("/Spacefox.png");
}

void TileMapLayer::createTileMapGLTexture() {

	glewInit();
	// //create frame buffer to render to a texture (final tiled sprite) This should not go here
	_fb = 0;	
	glGenFramebuffers(1, &_fb);
	glBindFramebuffer(GL_FRAMEBUFFER, _fb);
	
	//generate texture to render to
	glGenTextures(1, &_renderedTex);
	glBindTexture(GL_TEXTURE_2D, _renderedTex);

	glTexImage2D(GL_TEXTURE_2D, 0, _texAtlas->getTextureMode(), _layerW, _layerH, 0, _texAtlas->getTextureMode(), GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, 0);	
		
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _renderedTex, 0);
		
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	  	std::cout << "something bad happened" << std::endl;

	_tex_id = (void *)(intptr_t) _renderedTex;
}

void TileMapLayer::renderTile(int x, int y, int tileId) {

	float tX, tY, tX2, tY2;
	_texAtlas->getTileTexCoordinates(tileId, tX, tY, tX2, tY2);

	float x_pix = x * _texAtlas->getSpriteW();
	float y_pix = y * _texAtlas->getSpriteH();
	
	glBegin(GL_QUADS);	
	glTexCoord2f(tX, tY);
	glVertex3f(x_pix, y_pix, 0);	
	glTexCoord2f(tX2, tY);
	glVertex3f(x_pix + _texAtlas->getSpriteW(), y_pix, 0);	
	glTexCoord2f(tX2, tY2);
	glVertex3f(x_pix + _texAtlas->getSpriteW(), y_pix + _texAtlas->getSpriteH(), 0);	
	glTexCoord2f(tX, tY2);
	glVertex3f(x_pix, y_pix + _texAtlas->getSpriteH(), 0);	
	glEnd();

}

void TileMapLayer::updateTileMap(TileMap::Ptr tilemap) {

	// std::vector<int> tileIds = tilemap->getVisibleTiles(_ntilesW, _ntilesH);
}

void TileMapLayer::render() {

	glBindFramebuffer(GL_FRAMEBUFFER, _fb);
	glClearColor(1.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);	
//	glEnable(GL_ALPHA);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, _screenW, 0.0, _screenH, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _texAtlas->getTextureId());

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ZERO);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// std::vector<int> tileIds = tilemap->getVisibleTiles(_ntilesW, _ntilesH);
	// int c = 0;
	// for (int i = 0; i < _ntilesW; i++)
	// 	for (int j = 0; j < _ntilesH; j++) {
	// 		renderTile(i,j tileIds[c]);
	// 		c++;
	// 	}
	
	renderTile(0,0,5);
	renderTile(0,1,11);
	renderTile(0,4,13);
	renderTile(0,5,13);
	renderTile(0,6,13);
	renderTile(10,4,23);
	
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
