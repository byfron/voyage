#pragma once

#include <GL/glew.h>  //ensure that we include glew before gl.h
#include <networking/ClientNetworkManager.hpp>
#include "TileMap.hpp"

class SDLGraphicsEngine;

class GameClient {
public:
	~GameClient() {}
	void init();
	void start();		
	void clientLoop();
	void requestQuit();
 
	TileMap::Ptr getTileMap() { return _tileMap; };

	typedef std::shared_ptr<GameClient> Ptr;
	
private:

	std::shared_ptr<SDLGraphicsEngine> _graphicsEngine;
	ClientNetworkManager _netManager;
	TileMap::Ptr _tileMap;
	bool _running;
	
};
