#pragma once

#include <RakPeerInterface.h>
#include "PacketHandler.hpp"

using namespace RakNet;

class NetworkManager
{
public:
	NetworkManager() {}
	
	void registerHandler(PacketHandler::Ptr h, const std::list<int> & list) {

		h->isRegistered = true;

		for (auto m : list) {
			_msgHandlersMap[m] = h->getId();
		}
	
		_handlers[h->getId()] = h;	
	}

	void receiveData()
	{
		Packet *p;
		for (p=_peer->Receive(); p; _peer->DeallocatePacket(p), p=_peer->Receive())
		{
			int id = p->data[0];
			if (_msgHandlersMap.count(id))
				_handlers[_msgHandlersMap[id]]->onMessage(p);
		}
	}


protected:

	bool _isConnected;
	RakPeerInterface *_peer;
	std::map<int, int> _msgHandlersMap;	
	std::map<int, PacketHandler::Ptr> _handlers;
	int _port;
};

