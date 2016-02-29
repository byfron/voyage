#pragma once

#include "PacketHandler.hpp"

class ClientDataHandler : public PacketHandler {

public:

	void onPacket(Message *p) {};	
	
};
