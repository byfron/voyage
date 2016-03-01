#pragma once

#include <networking/ClientNetworkManager.hpp>

class GameClient {
public:
	void init();
	void start();		
	void clientLoop();
private:

	ClientNetworkManager _netManager;
	bool _finished;
	
};
