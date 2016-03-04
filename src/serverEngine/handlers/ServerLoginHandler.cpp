#include <networking/PacketHandler.hpp>
#include "ServerLoginHandler.hpp"
#include "handlers.hpp"
#include <spdlog/spdlog.h>

namespace spd = spdlog;

ServerLoginHandler::ServerLoginHandler(GameServer::Ptr gs) {
	_handlerId = 	SERVER_LOGIN_REQ_HANDLER;
	_gameServer = gs;
}

void ServerLoginHandler::onMessage(RakNet::Packet *p) {

	std::cout << "hey" << std::endl;

//	switch(p->data[
	
	// if logging is successful
//	_gameServer->createPlayerSession(m->getAddr());	
	
}
