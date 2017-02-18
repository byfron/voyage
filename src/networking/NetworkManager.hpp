#pragma once

#include <RakPeerInterface.h>
#include <list>
#include "PacketHandler.hpp"

using namespace RakNet;

class NetworkManager
{
public:
	NetworkManager() {}

	virtual ~NetworkManager() {}
	
	void registerHandler(PacketHandler::Ptr h, const std::list<int> & list) {

		h->isRegistered = true;

		for (auto m : list) {
			_msgHandlersMap[m] = h->getId();
		}
	
		_handlers[h->getId()] = h;	
	}

	RakNet::RakPeerInterface *getPeer() { return _peer; }
	
	void receiveData()
	{
		Packet *p;
		int max_msgs_per_frame = 8;
		int count = 0;
		for (p=_peer->Receive(); p && count < max_msgs_per_frame ; _peer->DeallocatePacket(p), p=_peer->Receive())
		{
			int id = p->data[0];
			if (_msgHandlersMap.count(id))
				_handlers[_msgHandlersMap[id]]->onMessage(p);

			count++;
		}

		return;

		
		// Packet *p = _peer->Receive();
		// if (p) {

		// 	int id = p->data[0];
		// 	if (_msgHandlersMap.count(id))
		// 		_handlers[_msgHandlersMap[id]]->onMessage(p);

		// 	// switch(id) {

		// 	// case ID_NEW_INCOMING_CONNECTION:
		// 	// {
		// 	// 	printf("ID_NEW_INCOMING_CONNECTION from %s. guid=%s.\n", p->systemAddress.ToString(true), p->guid.ToString());
		// 	// }
		// 	// break;
		// 	// case ID_CONNECTION_REQUEST_ACCEPTED:
		// 	// {
		// 	// 	printf("ID_CONNECTION_REQUEST_ACCEPTED from %s,guid=%s\n", p->systemAddress.ToString(true), p->guid.ToString());
		// 	// }
		// 	// break;
		// 	// default:
		// 	// {
		// 	// 	if (_msgHandlersMap.count(id))
		// 	// 		_handlers[_msgHandlersMap[id]]->onMessage(p);
		// 	// }

		// 	// }

		// 	_peer->DeallocatePacket(p);
		// }

	}

	typedef std::shared_ptr<NetworkManager> Ptr;


protected:

	bool _isConnected;
	RakPeerInterface *_peer;
	std::map<int, int> _msgHandlersMap;	
	std::map<int, PacketHandler::Ptr> _handlers;
	int _port;
};

