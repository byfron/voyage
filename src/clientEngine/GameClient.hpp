#pragma once

#include <networking/ClientNetworkManager.hpp>

class GameClient {
public:
	void init();
	void start();		

private:

	ClientNetworkManager _netManager;
	bool _finished;
	
};
