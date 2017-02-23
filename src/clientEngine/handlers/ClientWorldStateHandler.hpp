#pragma once

#include <networking/PacketHandler.hpp>
#include <common/handlers.hpp>
#include <common/GameMessages.hpp>
#include <voyage.pb.h>
#include <entities/EventManager.hpp>

class ClientWorldStateHandler : public PacketHandler {
public:
	ClientWorldStateHandler(ClientEngine::Ptr c) : _client(c) {

	}	

	void onMessage(RakNet::Packet *p) {

		Message<voyage::sc_worldState> msg(p);

		voyage::sc_worldState msgWorldState = msg.getContent();

		//push entity updates in the pool
		for (auto update : msgWorldState.entity_update()) {
			_client->msg_pool().pushEntityUpdateMsg(update);
		}
		
		//push player update in the pool
		if (msgWorldState.has_player_update()) {
			_client->msg_pool().pushPlayerUpdateMsg(msgWorldState.player_update());
	 	}
	}

private:
	ClientEngine::Ptr _client;	
};
