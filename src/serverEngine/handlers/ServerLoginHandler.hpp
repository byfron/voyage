#pragma once

#include <networking/PacketHandler.hpp>
#include <common/handlers.hpp>

class GameServer;

class ServerLoginHandler : public PacketHandler {

public:

	ServerLoginHandler(GameServer*);
	void onMessage(RakNet::Packet *p);

private:

	GameServer* _gameServer;
	
};
