#include "ServerDataHandler.hpp"
#include <common/handlers.hpp>
#include <spdlog/spdlog.h>
#include "voyage.pb.h"
#include <common/GameMessages.hpp>
#include "../PlayerSession.hpp"

namespace spd = spdlog;

//handlers: transform packets in Messages using protobuf stuff

ServerDataHandler::ServerDataHandler(GameServer* gs) {
	_handlerId = SERVER_USER_ACTION_HANDLER;
	_gameServer = gs;
}

void ServerDataHandler::onMessage(RakNet::Packet * p) {

	spd::get("Server")->info() << "Received data:" << p->data[0];
	PlayerSession::Ptr ps;
	
	switch(p->data[0]) {

	case ID_CS_USER_MOVEMENT: {
		Message<voyage::cs_userMovement>::Ptr m =
			std::make_shared< Message<voyage::cs_userMovement> >(p);

//		_gameServer->
		// push this in body system
		break;
		
	}
		
	}
	
		
	//test:run script
	// ScriptEngine::Ptr se = _gameServer->getScriptEngine();

	// se->runScript(ps, SCRIPTS_DIR "/test.py");
	
	//send message to the corresponding playerSession	

}

