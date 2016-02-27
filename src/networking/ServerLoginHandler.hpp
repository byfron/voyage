#pragma once

#include "PacketHandler.hpp"

class ServerLoginHandler : public PacketHandler {

public:

	HandlerId getType() { return SERVER_LOGIN_HANDLER; }       
	void onPacket(LoginMessage *m);
	
};
