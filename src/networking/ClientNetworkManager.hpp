#ifndef __VOYAGE_CLIENT_HPP__
#define __VOYAGE_CLIENT_HPP__

//Tiny client
#include <RakPeerInterface.h>
//#include "PacketHandler.hpp"
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
	void registerHandler(PacketHandler::Ptr h, const std::list<int> & list);
	void receiveData();

	template<typename T>
	void sendData(RakNet::MessageID id, const T & content) {
		
		RakNet::BitStream bsOut;
		std::shared_ptr< Message<T> > m = std::make_shared< Message<T> >(id, content);
		m->toBitStream(bsOut);
		_peer->Send(&bsOut, HIGH_PRIORITY,RELIABLE_ORDERED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
		
	}
//	void sendData(RakNet::MessageID id);

	typedef std::shared_ptr<ClientNetworkManager> Ptr;

private:

	bool _isConnected;
	RakPeerInterface *_peer;
	std::map<int, int> _msgHandlersMap;	
	std::map<int, PacketHandler::Ptr> _handlers;
	int _port;
	const char *_remoteIPAddress;
};

#endif
