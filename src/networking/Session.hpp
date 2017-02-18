#pragma once

#include <RakPeerInterface.h>
#include <networking/Messages.hpp>
#include <vector>
#include <map>

class Session {
	
public:

	typedef std::shared_ptr<Session> Ptr;

	Session(RakNet::RakNetGUID guid, uint16_t player_id) : _guid(guid), _playerId(player_id) { }

	unsigned int getPlayerId() { return _playerId; }
	
	template <typename T>
	void sendRakNetMessage(RakNet::RakPeerInterface *peer, std::shared_ptr< Message<T> >  m) {
		RakNet::BitStream bsOut;
		m->toBitStream(bsOut);

		if (peer)
		peer->Send(&bsOut, HIGH_PRIORITY,
			   RELIABLE_ORDERED, 0,
			   _guid, false);
	};
	
protected:
	
	std::string _name;
	std::string _playerLogin;
	std::string _playerPwd;
	RakNet::RakNetGUID _guid;
	unsigned int _playerId;
	RakNet::Time _creationTime;
	
};

class SessionPool {

private:

	std::map<std::string, Session::Ptr> _sessionAddrMap;
	std::vector<Session::Ptr> _playerSessions;
};
