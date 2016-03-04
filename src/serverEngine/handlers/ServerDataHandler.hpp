#pragma once

#include <networking/PacketHandler.hpp>
#include "../GameServer.hpp"
#include "handlers.hpp"

class ServerDataHandler : public PacketHandler {

public:
	ServerDataHandler(GameServer::Ptr);	
	void onMessage(RakNet::Packet *);

private:

	GameServer::Ptr _gameServer;
	
};

