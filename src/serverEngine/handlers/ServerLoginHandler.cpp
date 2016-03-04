#include <networking/PacketHandler.hpp>
#include "ServerLoginHandler.hpp"
#include "handlers.hpp"
#include <spdlog/spdlog.h>
#include <common/GameMessages.hpp>
#include "voyage.pb.h"

namespace spd = spdlog;

ServerLoginHandler::ServerLoginHandler(GameServer::Ptr gs) {
	_handlerId = 	SERVER_LOGIN_REQ_HANDLER;
	_gameServer = gs;
}

void ServerLoginHandler::onMessage(RakNet::Packet *p) {

	switch(p->data[0]) {
			
	case ID_CS_LOGIN_REQUEST:
		Message<voyage::cs_LoginRequest>::Ptr m =
			std::make_shared< Message<voyage::cs_LoginRequest> >(p);
		spd::get("Server")->info() << "Login Request";
		
		break;
	}
	// if logging is successful
//	_gameServer->createPlayerSession(m->getAddr());	
	
}
