#include "ServerDataHandler.hpp"
#include "handlers.hpp"
#include <spdlog/spdlog.h>
#include "voyage.pb.h"

namespace spd = spdlog;

//handlers: transform packets in Messages using protobuf stuff

ServerDataHandler::ServerDataHandler(GameServer::Ptr gs) {
	_handlerId = SERVER_USER_ACTION_HANDLER;
	_gameServer = gs;
}
void ServerDataHandler::onMessage(RakNet::Packet * p) {

	spd::get("Server")->info() << "Received data:" << p->data[0];
	PlayerSession::Ptr ps;
	
	switch(p->data[0]) {

	case ID_LOGIN_MESSAGE:
		Message<voyage::cs_LoginRequest>::Ptr m =
			std::make_shared< Message<voyage::cs_LoginRequest> >(p);
		ps = _gameServer->getPlayerSession(m->getAddr());
		break;
	}
	
		
	//test:run script
	ScriptEngine::Ptr se = _gameServer->getScriptEngine();

	se->runScript(ps, SCRIPTS_DIR "/test.py");
	
	//send message to the corresponding playerSession	

}

