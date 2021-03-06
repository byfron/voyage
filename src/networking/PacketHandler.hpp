#pragma once

#include <iostream>
#include <memory>
#include "Messages.hpp"

class ServerNetworkManager;

class PacketHandler {

public:
	PacketHandler() { isRegistered = false; }
	virtual ~PacketHandler() {};	
	virtual void onMessage(RakNet::Packet *p) { assert(false); };

	int getId() { return _handlerId; }
	typedef std::shared_ptr<PacketHandler> Ptr;

	bool isRegistered;
	
protected:

	int _handlerId;
	
};



