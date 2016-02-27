#pragma once

#include "PacketHandler.hpp"

class ClientDataHandler : public PacketHandler {

public:

	void onPacket(DataMessage *p) {};
	HandlerId getType() { return CLIENT_DATA_HANDLER; }
	
};
