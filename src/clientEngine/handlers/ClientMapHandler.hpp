#pragma once
#include <networking/PacketHandler.hpp>
#include "../GameClient.hpp"
#include <common/handlers.hpp>
#include <common/GameMessages.hpp>
#include "voyage.pb.h"

class ClientMapHandler  : public PacketHandler {
public:
	ClientMapHandler(GameClient::Ptr c) {
		_handlerId = CLIENT_MAP_HANDLER;
		_client = c;
	}

	void onMessage(RakNet::Packet *p) {

		std::cout << "received tiles" << std::endl;
	}

private:

	GameClient::Ptr _client;
	
};
