#ifndef __VOYAGE_SERVER_HPP__
#define __VOYAGE_SERVER_HPP__

#include <RakPeerInterface.h>
#include <map>
#include <list>
#include "PacketHandler.hpp"
#include "Session.hpp"
#include "NetworkManager.hpp"

#define SERVER_PORT 1234

class ServerNetworkManager : public NetworkManager
{
public:
	ServerNetworkManager();
	~ServerNetworkManager();	
	void start(const int port);
	void stop();

	unsigned connectionCount(void) const;

	RakNet::RakPeerInterface *getPeer() { return _peer; }
	typedef std::shared_ptr<ServerNetworkManager> Ptr;
	
private:
	bool _server_running;
};

#endif
