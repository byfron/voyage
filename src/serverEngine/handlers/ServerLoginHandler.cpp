#include <networking/PacketHandler.hpp>
#include "ServerLoginHandler.hpp"
#include "handlers.hpp"
#include <spdlog/spdlog.h>

namespace spd = spdlog;

ServerLoginHandler::ServerLoginHandler(GameServer::Ptr gs) {
	_handlerId = SERVER_LOGIN_HANDLER;
	_gameServer = gs;
}



void ServerLoginHandler::onMessage(Message::Ptr m) {

	spd::get("console")->info() << "Creating session: " << *m;
	

	// if logging is successful
//	_gameServer->createPlayerSession();	
	
	
}
