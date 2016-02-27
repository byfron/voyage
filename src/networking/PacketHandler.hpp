#pragma once

#include <iostream>
#include <memory>
#include "Messages.hpp"

enum HandlerId {
	CLIENT_LOGIN_HANDLER,
	CLIENT_DATA_HANDLER,
	SERVER_LOGIN_HANDLER,
	SERVER_DATA_HANDLER
};

class PacketHandler {

public:
	PacketHandler() {};
	virtual ~PacketHandler() {};
	virtual HandlerId getType() {};
	void onPacket(std::shared_ptr<LoginMessage> l) {};
	void onPacket(std::shared_ptr<DataMessage> d) {};
};

typedef std::shared_ptr<PacketHandler> PacketHandlerPtr;
