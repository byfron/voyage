#pragma once

#include <networking/ClientNetworkManager.hpp>

class SDLGraphicsEngine;

class GameClient {
public:
	void init();
	void start();		
	void clientLoop();
	void requestQuit();

	typedef std::shared_ptr<GameClient> Ptr;
	
private:

	std::shared_ptr<SDLGraphicsEngine> _graphicsEngine;
	ClientNetworkManager _netManager;
	bool _running;
	
};
