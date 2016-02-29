#pragma once

#include <RakPeerInterface.h>
#include <networking/ServerNetworkManager.hpp>
#include <scripts/ScriptEngine.hpp>
#include <sql/DatabaseConnectionPool.hpp>
#include "PlayerSession.hpp"

class GameServer {
public:
	GameServer(int port, int poolSize);

	void start();
	void stop();
	int createPlayerSession(RakNet::SystemAddress);
	

	PlayerSession::Ptr getPlayerSession(RakNet::SystemAddress addr) {
		return _playerSessionAddrMap[addr];
	}
	
	RakNet::RakPeerInterface *getRakNetPeer() { _netManager.getPeer(); }

	typedef std::shared_ptr<GameServer> Ptr;
	
	
private:
	void _init(int port);
	
	DatabaseConnectionPool _dbPool;
	ScriptEngine _scriptEngine;
	ServerNetworkManager _netManager;

	std::vector<PlayerSession::Ptr> _playerSessions;
	std::map<RakNet::SystemAddress, PlayerSession::Ptr> _playerSessionAddrMap; 
	
	bool _finished;
	std::string _datasetName;
};

