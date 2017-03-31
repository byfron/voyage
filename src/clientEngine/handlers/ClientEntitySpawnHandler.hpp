#include <networking/PacketHandler.hpp>
#include "../ClientEngine.hpp"
#include <common/handlers.hpp>
#include <common/GameMessages.hpp>
#include "voyage.pb.h"

class ClientEntitySpawnHandler : public PacketHandler {

public:

	ClientEntitySpawnHandler(ClientEngine::Ptr c) {
		_handlerId = CLIENT_ENTITY_SPAWN_HANDLER;
		_client = c;
	}

	void onMessage(RakNet::Packet *p) {

		switch (p->data[0])
		{

		case ID_SC_SPAWN_PLAYER: {
			Message<voyage::sc_entitySpawn> msg(p);
			voyage::sc_entitySpawn data = msg.getContent();
			_client->createPlayer(data.entityid(), data.x(), data.y());
			spd::get("Client")->info() << "Spawning player";
			break;
		}

		case ID_SC_SPAWN_ENTITY: {
			Message<voyage::sc_entitySpawn> msg(p);
			break;
		}

		// case ID_SC_DESTROY_ENTITY: {

		// 	Message<voyage::sc_entityDestroy> msg(p);
		// 	break;
		// }

		// PUT here player spawn too?

		};

	}

private:
	ClientEngine::Ptr _client;
};
