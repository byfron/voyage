#include <networking/PacketHandler.hpp>
#include <common/handlers.hpp>
#include <spdlog/spdlog.h>
#include <common/GameMessages.hpp>
#include <serverEngine/PlayerSession.hpp>
#include <serverEngine/GameServer.hpp>
#include <serverEngine/MessageFactory.hpp>
#include <entities/Entity.hpp>
#include "ServerLoginHandler.hpp"
#include "voyage.pb.h"

namespace spd = spdlog;
using namespace voyage;

ServerLoginHandler::ServerLoginHandler(GameServer *gs) {
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
		PlayerSession::Ptr session = _gameServer->createPlayerSession(p->guid);

		// Acknowledge login successful and send important data:
		// TODO: 
		// - player_id
		// - player name/data/etc...

		Entity pe = _gameServer->engine().createPlayerEntity(session->getPlayerId());
		session->setPlayerEntityId(pe.id().id);				
		sc_loginAccepted login_ack;
		login_ack.set_playerid(pe.id().id);
		Message<sc_loginAccepted>::Ptr msg = std::make_shared<Message<sc_loginAccepted> >(ID_SC_LOGIN_ACCEPTED, login_ack);
		session->sendMessage(msg);	

		// Create player entity and send spawn message to client
		BodyComponent *body = _gameServer->engine().entity_manager().
			getComponentPtr<BodyComponent>(pe.id());

		Message<sc_entitySpawn>::Ptr spawn_msg =
		MessageFactory::createSpawnPlayerMsg(pe.id().id, *body);
		
		// send its spawn to all the other sessions
		_gameServer->broadcastMessageToAll(spawn_msg);

		// spawn every other player entity to the session
		_gameServer->engine().entity_manager().
			each<NetworkComponent, PlayerComponent, BodyComponent>([session](Entity entity,
								       NetworkComponent &netdata,
								       PlayerComponent &player,
								       BodyComponent &body) {

		     if (session->getPlayerEntityId() != entity.id().id) {			 
			     Message<sc_entitySpawn>::Ptr spawn_msg =
				     MessageFactory::createSpawnPlayerMsg(entity.id().id, body);
			     session->sendMessage(spawn_msg);
		     }
		     });
		
		break;
	}

	
}
