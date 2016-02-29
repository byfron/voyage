#pragma once

#include <networking/Session.hpp>
#include "GameServer.hpp"

class PlayerSession : public Session {

public:
	
	typedef std::shared_ptr<PlayerSession> PlayerSessionPtr;

	PlayerSession() {}
	PlayerSession(GameServer::Ptr _gs);
	
	void sendMessage(Message::Ptr m) {
		if (_gameServer)
		       sendRakNetMessage(_gameServer->getRakNetPeer(), m);
	}
	
private:

	GameServer::Ptr _gameServer;
	DatabaseConnectionPool::Ptr _dbPool;
};
