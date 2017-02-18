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

		switch (p->data[0])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED: {
			printf("ID_CONNECTION_REQUEST_ACCEPTED\n");

			// send custon login message. This may not be needed. 
			voyage::cs_loginRequest logreq;
			logreq.set_username("byfron");
			logreq.set_password("1234");
			_client->getNetManager().sendData<voyage::cs_loginRequest>(ID_CS_LOGIN_REQUEST, logreq);

			
			break;
		}
		case ID_SC_LOGIN_ACCEPTED: {
			Message<voyage::sc_loginAccepted> msg(p);
			
			voyage::sc_loginAccepted data = msg.getContent();
			
			std::cout << "Login accepted. Player id " << data.playerid() << std::endl;
			
			GameEngine::m_playerId = data.playerid();
			break;
		}
		};
	}

private:
	ClientEngine::Ptr _client;
};
