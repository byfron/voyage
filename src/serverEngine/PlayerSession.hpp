#pragma once

#include <networking/Session.hpp>
#include <database/DatabaseConnectionPool.hpp>

class GameServer;
typedef std::shared_ptr<GameServer> GameServerPtr;

class PlayerSession : public Session {

public:

	typedef std::shared_ptr<PlayerSession> Ptr;

	PlayerSession(unsigned int id, RakNet::SystemAddress addr, GameServerPtr _gs);

	template <typename T>
	void sendMessage(std::shared_ptr< Message<T> > );

	int getId() { return _id; }
	
private:

	unsigned int _id;
	RakNet::SystemAddress _addr;
	
	GameServerPtr _gameServer;
	DatabaseConnectionPool::Ptr _dbPool;
};
