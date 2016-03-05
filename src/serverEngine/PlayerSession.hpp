#pragma once

#include <networking/Session.hpp>
#include <database/DatabaseConnectionPool.hpp>
#include "GameServer.hpp"

class PlayerSession : public Session {

public:

	typedef std::shared_ptr<PlayerSession> Ptr;

	PlayerSession(unsigned int id, RakNet::SystemAddress addr, GameServer::Ptr _gs);

	template <typename T>
	void sendMessage(std::shared_ptr< Message<T> > m) {
		sendRakNetMessage(_gameServer->getRakNetPeer(), m);
	}


	int getId() { return _id; }
	
private:

	unsigned int _id;
	RakNet::SystemAddress _addr;
	
	GameServer::Ptr _gameServer;
	DatabaseConnectionPool::Ptr _dbPool;
};
