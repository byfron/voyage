#pragma once

#include <networking/PacketHandler.hpp>
#include "../GameServer.hpp"
#include <common/handlers.hpp>

class ServerDataHandler : public PacketHandler {

public:
	ServerDataHandler(GameServer*);	
	void onMessage(RakNet::Packet *);

private:

	GameServer* _gameServer;
	
};

