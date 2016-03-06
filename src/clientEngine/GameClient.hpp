#pragma once

#include <GL/glew.h>  //ensure that we include glew before gl.h
#include <networking/ClientNetworkManager.hpp>
#include "PlayerData.hpp"
#include "ClientTileMap.hpp"

class SDLGraphicsEngine;

class GameClient {
public:
	~GameClient() {}
	void init();
	void start();		
	void clientLoop();
	void requestQuit();

	void initPlayer(PlayerData & playerData);
	
	ClientTileMap::Ptr getTileMap() { return _tileMap; };
	ClientNetworkManager::Ptr getNetworkManager() { return _netManager; }
	typedef std::shared_ptr<GameClient> Ptr;
	
private:

	void _registerHandlers();
	
	std::shared_ptr<SDLGraphicsEngine> _graphicsEngine;
	ClientNetworkManager::Ptr _netManager;
	ClientTileMap::Ptr _tileMap;
	bool _running;
	
};
