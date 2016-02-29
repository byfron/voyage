#ifndef __VOYAGE_SERVER_HPP__
#define __VOYAGE_SERVER_HPP__

#include <RakPeerInterface.h>
#include <map>
#include <list>
#include "PacketHandler.hpp"
#include "Session.hpp"

#define SERVER_PORT 1234

class ServerNetworkManager
{
public:
	ServerNetworkManager();
	~ServerNetworkManager();	
	void start(const int port);
	void stop();
	void receiveData();
	void registerHandler(PacketHandler::Ptr, const std::list<int> & list);
	unsigned connectionCount(void) const;
	void createSession();
	void removeSession();

	RakNet::RakPeerInterface *getPeer() { return _peer; }
	typedef std::shared_ptr<ServerNetworkManager> Ptr;
	
private:
	RakNet::RakPeerInterface *_peer;
	std::map<int, int> _msgHandlersMap;
	std::map<int, PacketHandler::Ptr> _handlers;
	bool _server_running;
};

#endif
