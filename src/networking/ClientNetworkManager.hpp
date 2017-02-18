#pragma once

//Tiny client
#include <RakPeerInterface.h>
//#include "PacketHandler.hpp"
#include <map>
#include "ServerNetworkManager.hpp"
#include "NetworkManager.hpp"

using namespace RakNet;


class ClientNetworkManager  : public NetworkManager
{
public:
	ClientNetworkManager();
	~ClientNetworkManager();
	void start(const char *address, const int port);
	void connect();
	void disconnect();

	void sendCustomLoginRequest();

	template<typename T>
	void sendData(RakNet::MessageID id, const T & content) {
		
		RakNet::BitStream bsOut;
		std::shared_ptr< Message<T> > m = std::make_shared< Message<T> >(id, content);
		m->toBitStream(bsOut);
		_peer->Send(&bsOut, HIGH_PRIORITY,RELIABLE_ORDERED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
		
	}

	typedef std::shared_ptr<ClientNetworkManager> Ptr;

private:

	const char *_remoteIPAddress;
};

