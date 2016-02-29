#pragma once

#include <networking/PacketHandler.hpp>
#include "../GameServer.hpp"
#include "handlers.hpp"

class ServerLoginHandler : public PacketHandler {

public:

	ServerLoginHandler(GameServer::Ptr);
	void onMessage(Message::Ptr m);

private:

	GameServer::Ptr _gameServer;
	
};
