#ifndef __VOYAGE_SERVER_HPP__
#define __VOYAGE_SERVER_HPP__

#include <RakPeerInterface.h>
#include <map>
#include "PacketHandler.hpp"

#define SERVER_PORT 1234

class ServerNetworkManager
{
public:
	ServerNetworkManager();
	~ServerNetworkManager();	
	void start();
	void stop();
	void receiveData();
	void registerHandler(PacketHandlerPtr);

private:
	RakNet::RakPeerInterface *_peer;
	std::map<HandlerId, PacketHandlerPtr> _handlers;
	bool _server_running;
};

#endif
