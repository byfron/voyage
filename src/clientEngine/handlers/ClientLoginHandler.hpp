#pragma once

#include <networking/PacketHandler.hpp>
#include "../ClientEngine.hpp"
#include <common/handlers.hpp>
#include <common/GameMessages.hpp>
#include "voyage.pb.h"

class ClientLoginHandler : public PacketHandler {

public:

	ClientLoginHandler(ClientEngine::Ptr c) {
		_handlerId = CLIENT_LOGIN_HANDLER;
		_client = c;
	}
	
	void onMessage(RakNet::Packet *p) {
		
		std::cout << "login successful"  << std::endl;
		Message<voyage::sc_loginAccepted> msg(p);
		
		//get the player info
		//_client->initPlayer();

		//DEBUG: ask server for map data
		//TODO: _client->requestMapRegion();
		voyage::cs_regionDataRequest msgRegReq;
		msgRegReq.set_x(25);
		msgRegReq.set_y(25);
		//msg.set_timestamp(); of the last seen region!!
		_client->getNetworkManager()->sendData<voyage::cs_regionDataRequest>(ID_CS_REGION_REQUEST, msgRegReq);
		
	}

private:
	ClientEngine::Ptr _client;
};
