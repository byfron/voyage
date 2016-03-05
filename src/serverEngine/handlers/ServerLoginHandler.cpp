#include <networking/PacketHandler.hpp>
#include "ServerLoginHandler.hpp"
#include <common/handlers.hpp>
#include <spdlog/spdlog.h>
#include <common/GameMessages.hpp>
#include "voyage.pb.h"
#include "../PlayerSession.hpp"

namespace spd = spdlog;
using namespace voyage;

ServerLoginHandler::ServerLoginHandler(GameServer::Ptr gs) {
	_handlerId = SERVER_LOGIN_REQ_HANDLER;
	_gameServer = gs;
}

void ServerLoginHandler::onMessage(RakNet::Packet *p) {

	switch(p->data[0]) {
			
	case ID_CS_LOGIN_REQUEST:		
		Message<cs_loginRequest>::Ptr m =
			std::make_shared< Message<cs_loginRequest> >(p);
		spd::get("Server")->info() << "Login Request";

		// if logging is successful
		PlayerSession::Ptr session = _gameServer->createPlayerSession(m->getAddr());

		//send relevant data to the client
		sc_loginGranted content;
		Message<sc_loginGranted>::Ptr msg = std::make_shared<Message<sc_loginGranted> >(ID_SC_LOGIN_ACCEPTED, content);		
		session->sendMessage<sc_loginGranted>(msg);
		
		break;
	}

	
}
