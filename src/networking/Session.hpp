#pragma once

#include <RakPeerInterface.h>
#include <networking/Messages.hpp>
#include <vector>
#include <map>

class Session {
	
public:

	typedef std::shared_ptr<Session> Ptr;
	void sendRakNetMessage(RakNet::RakPeerInterface *peer, Message::Ptr m) {
		RakNet::BitStream bsOut;
		m->toBitStream(bsOut);
		peer->Send(&bsOut, HIGH_PRIORITY,
			   RELIABLE_ORDERED, 0,
			   _address, true);
	};
	
private:
	
	std::string _name;
	std::string _playerLogin;
	std::string _playerPwd;
	RakNet::AddressOrGUID _address;
	unsigned int _playerId;
	RakNet::Time _creationTime;
	
};

class SessionPool {

private:

	std::map<std::string, Session::Ptr> _sessionAddrMap;
	std::vector<Session::Ptr> _playerSessions;
};
