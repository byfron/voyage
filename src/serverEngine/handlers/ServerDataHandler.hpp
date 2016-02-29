#pragma once

#include <networking/PacketHandler.hpp>

class ServerDataHandler : public PacketHandler {

public:

	ServerDataHandler();	
	void onMessage(Message *p);
	
};
