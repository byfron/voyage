#pragma once

#include <networking/Session.hpp>
#include <database/DatabaseConnectionPool.hpp>


class PlayerSession : public Session {

public:

	typedef std::shared_ptr<PlayerSession> Ptr;

	PlayerSession(unsigned int id, RakNet::RakNetGUID guid, RakNet::RakPeerInterface *peer) :
		_peer(peer), Session(guid, id) {}

	template <typename T>
	void sendMessage(std::shared_ptr< Message<T> > m) {		
		sendRakNetMessage(_peer, m);
	}

	void setPlayerEntityId(uint32_t id) {
		m_entity_id = id;
	}

	uint32_t getPlayerEntityId() {
		return m_entity_id;
	}
	
private:

	uint32_t m_entity_id;

//	GameServer * _gameServer;
	RakNet::RakPeerInterface *_peer;
//	DatabaseConnectionPool::Ptr _dbPool;
};
