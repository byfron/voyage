#pragma once

#include <networking/Session.hpp>
#include <database/DatabaseConnectionPool.hpp>
#include "GameServer.hpp"

class PlayerSession : public Session {

public:

	typedef std::shared_ptr<PlayerSession> Ptr;

	PlayerSession(unsigned int id, RakNet::RakNetGUID guid, GameServer & gs) :
		_gameServer(gs), Session(guid, id) {}

	template <typename T>
	void sendMessage(std::shared_ptr< Message<T> > m) {
		
		sendRakNetMessage(_gameServer.getRakNetPeer(), m);
	}
	
private:

	GameServer & _gameServer;
//	DatabaseConnectionPool::Ptr _dbPool;
};
