#pragma once

#include <networking/PacketHandler.hpp>
#include "GameClient.hpp"
#include <common/handlers.hpp>

class ClientLoginHandler : public PacketHandler {

public:

	ClientLoginHandler(GameClient::Ptr c) {
		_handlerId = CLIENT_LOGIN_HANDLER;
		_client =c; }
	void onMessage(RakNet::Packet *p) {
		
		std::cout << "login successfuly"  << std::endl;
	}

private:
	GameClient::Ptr _client;
};
