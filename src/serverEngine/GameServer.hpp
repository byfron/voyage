#pragma once

#include <RakPeerInterface.h>
#include "ServerEngine.hpp"
#include <spdlog/spdlog.h>

class PlayerSession;
typedef std::shared_ptr<PlayerSession> PlayerSessionPtr;

class GameServer {
public:
	GameServer(int port);

	void start();
	void stop();
	PlayerSessionPtr createPlayerSession(RakNet::RakNetGUID guid);
	void broadcastWorldState();

	PlayerSessionPtr getPlayerSession(RakNet::RakNetGUID guid) {
		if (_playerSessionAddrMap.count(guid) == 0) {
			spd::get("Server")->warn() <<
				"Player session does not exist in addr:" << guid.ToString();
			return nullptr;
		}
		return _playerSessionAddrMap[guid];
	}

	RakNet::RakPeerInterface *getRakNetPeer() { _gameEngine.getNetManager().getPeer(); }

	ServerEngine & engine() { return _gameEngine; }
	
	typedef std::shared_ptr<GameServer> Ptr;
	
private:
	std::vector<PlayerSessionPtr> _playerSessions;
	std::map<RakNet::RakNetGUID, PlayerSessionPtr> _playerSessionAddrMap; 
	
	bool _finished = false;
	ServerEngine _gameEngine;
	

};

