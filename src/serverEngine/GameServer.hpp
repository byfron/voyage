#pragma once

#include <RakPeerInterface.h>
#include <networking/ServerNetworkManager.hpp>

class GameServer {
public:
	void init(int);
	void start();

	RakNet::RakPeerInterface *getRakNetPeer() { _netManager.getPeer(); }
	
	typedef std::shared_ptr<GameServer> Ptr;
private:

	ServerNetworkManager _netManager;		 
	bool _finished;
};
