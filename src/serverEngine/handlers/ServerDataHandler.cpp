#include "ServerDataHandler.hpp"
#include "handlers.hpp"
#include <spdlog/spdlog.h>

namespace spd = spdlog;

ServerDataHandler::ServerDataHandler(GameServer::Ptr gs) {
	_handlerId = SERVER_DATA_HANDLER;
	_gameServer = gs;
}
void ServerDataHandler::onMessage(Message::Ptr m) {

	spd::get("Server")->info() << "Received data:" << *m;


	//send message to the corresponding playerSession
	

}

