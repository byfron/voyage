#include <networking/PacketHandler.hpp>
#include "ServerLoginHandler.hpp"
#include <common/handlers.hpp>
#include <spdlog/spdlog.h>
#include <common/GameMessages.hpp>
#include "voyage.pb.h"
#include "../PlayerSession.hpp"
#include <serverEngine/GameServer.hpp>
#include <serverEngine/MessageFactory.hpp>

namespace spd = spdlog;
using namespace voyage;

ServerLoginHandler::ServerLoginHandler(GameServer *gs) {
	_handlerId = SERVER_LOGIN_REQ_HANDLER;
	_gameServer = gs;


	// DEVELOPEMENT: Add a default session on 127.0.0.1
	// PlayerSession::Ptr session =
	// 	_gameServer->createPlayerSession(RakNet::SystemAddress("127.0.0.1"));

}

void ServerLoginHandler::onMessage(RakNet::Packet *p) {

	switch(p->data[0]) {
			
	case ID_CS_LOGIN_REQUEST:

		std::cout << "RECEIVING LOGIN REQ" << std::endl;
		
		Message<cs_loginRequest>::Ptr m =
			std::make_shared< Message<cs_loginRequest> >(p);
		spd::get("Server")->info() << "Login Request";

		// if logging is successful
		PlayerSession::Ptr session = _gameServer->createPlayerSession(p->guid);

		// Acknowledge login successful and send important data:
		// - player_id
		// - player name/data/etc...

		Entity pe = _gameServer->engine().createPlayerEntity(session->getPlayerId());
		session->setPlayerEntityId(pe.id().id);
		sc_loginAccepted login_ack;
		login_ack.set_playerid(pe.id().id);
		Message<sc_loginAccepted>::Ptr msg = std::make_shared<Message<sc_loginAccepted> >(ID_SC_LOGIN_ACCEPTED, login_ack);
		session->sendMessage(msg);	

		// Create player entity and send spawn message to client
		BodyCmp *body = _gameServer->engine().entity_manager().
			getComponentPtr<BodyCmp>(pe.id());



		Message<sc_entitySpawn>::Ptr spawn_msg =
		MessageFactory::createSpawnPlayerMsg(pe.id().id, body);
			       

		// send its spawn to all the other sessions
		_gameServer->broadcastMessageToAllButThis(session->getPlayerId(), spawn_msg);

		// send spawn to every other player
		
		session->sendMessage(spawn_msg);
		
		// spawn every networked entity
//		session->sendMessage(spawn_msg);

		// 

		break;
	}

	
}
