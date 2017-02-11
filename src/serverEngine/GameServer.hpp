#pragma once

#include <RakPeerInterface.h>
#include "ServerEngine.hpp"

class PlayerSession;
typedef std::shared_ptr<PlayerSession> PlayerSessionPtr;

class GameServer {
public:
	GameServer(int port);

	void start();
	void stop();
	PlayerSessionPtr createPlayerSession(RakNet::SystemAddress);
	

	PlayerSessionPtr getPlayerSession(RakNet::SystemAddress addr) {
		return _playerSessionAddrMap[addr];
	}
	
	RakNet::RakPeerInterface *getRakNetPeer() { _gameEngine.networkManager()->getPeer(); }
	
	typedef std::shared_ptr<GameServer> Ptr;
	
private:
	std::vector<PlayerSessionPtr> _playerSessions;
	std::map<RakNet::SystemAddress, PlayerSessionPtr> _playerSessionAddrMap; 
	
	bool _finished;
	ServerEngine _gameEngine;
	

};

