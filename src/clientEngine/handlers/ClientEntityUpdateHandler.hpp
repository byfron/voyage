#pragma once


#include <networking/PacketHandler.hpp>
#include <common/handlers.hpp>
#include <common/GameMessages.hpp>
#include <voyage.pb.h>
#include <entities/EventManager.hpp>

// Receives update data from entity/component 
class ClientEntityUpdateHandler : public PacketHandler {
public:
	ClientEntityUpdateHandler(EventManager *em) : m_event_manager(em) {

	}	

	void onMessage(RakNet::Packet *p) {

		Message<voyage::sc_entityUpdate> msg(p);

		voyage::sc_entityUpdate msgEntUpdate;

		// unpack message and send event to corresponding entity
		m_event_manager->emit<voyage::sc_entityUpdate>(msgEntUpdate);		
	}

private:
	EventManager *m_event_manager;
};
