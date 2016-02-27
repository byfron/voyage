#pragma once

#include "PacketHandler.hpp"

class ServerDataHandler : public PacketHandler {

public:

	void onPacket(DataMessage *p) {};
	HandlerId getType() { return SERVER_DATA_HANDLER; }
	
};
