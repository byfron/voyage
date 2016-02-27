#ifndef __VOYAGE_CLIENT_HPP__
#define __VOYAGE_CLIENT_HPP__

//Tiny client
#include <RakPeerInterface.h>
#include "PacketHandler.hpp"
#include <map>
#include "ServerNetworkManager.hpp"

using namespace RakNet;


class ClientNetworkManager
{
public:
	ClientNetworkManager();
	~ClientNetworkManager();
	void start();
	void connect();
	void disconnect();
	void registerHandler(PacketHandlerPtr h);
	void receiveData();

private:
	bool _isConnected;
	RakPeerInterface *_peer;
	std::map<HandlerId, PacketHandlerPtr> _handlers;
	char *_remoteIPAddress;
};

#endif
