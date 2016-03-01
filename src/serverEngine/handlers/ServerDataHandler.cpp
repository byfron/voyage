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
	
	PlayerSession::Ptr ps = _gameServer->getPlayerSession(m->getAddr());

	//test:run script
	ScriptEngine::Ptr se = _gameServer->getScriptEngine();

	se->runScript(ps, SCRIPTS_DIR "/test.py");
	
	//send message to the corresponding playerSession	

}

