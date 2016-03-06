#pragma once

#include <common/handlers.hpp>
#include "../GameServer.hpp"
#include "voyage.pb.h"
#include <serverEngine/PlayerSession.hpp>

//TODO make this derive from PacketHandler and include already all the game server pointer thing
class ServerMapHandler : public PacketHandler {

public:
	ServerMapHandler(GameServer::Ptr gs);
	void onMessage(RakNet::Packet *p);
	void handleRegionDataRequest(PlayerSession::Ptr session, const voyage::cs_regionDataRequest & content);
	
private:
	GameServer::Ptr _gameServer;
	
};
