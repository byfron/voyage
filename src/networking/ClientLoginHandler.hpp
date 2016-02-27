#pragma once

#include "PacketHandler.hpp"

class ClientLoginHandler : public PacketHandler {

public:

	HandlerId getType() { return CLIENT_LOGIN_HANDLER; }       
	void onPacket(LoginMessage *m);
	
};
