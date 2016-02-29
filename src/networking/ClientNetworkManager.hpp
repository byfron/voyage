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
	void start(const char *address, const int port);
	void connect();
	void disconnect();
	void registerHandler(PacketHandler::Ptr h);
	void receiveData();
	void sendData(MessageId id, const char *data);
	void sendData(MessageId id);

private:

	bool _isConnected;
	RakPeerInterface *_peer;
	std::map<int, PacketHandler::Ptr> _handlers;
	int _port;
	const char *_remoteIPAddress;
};

#endif
