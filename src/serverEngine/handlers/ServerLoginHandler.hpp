#pragma once

#include <networking/PacketHandler.hpp>
#include "../GameServer.hpp"
#include <common/handlers.hpp>

class ServerLoginHandler : public PacketHandler {

public:

	ServerLoginHandler(GameServer::Ptr);
	void onMessage(RakNet::Packet *p);

private:

	GameServer::Ptr _gameServer;
	
};
