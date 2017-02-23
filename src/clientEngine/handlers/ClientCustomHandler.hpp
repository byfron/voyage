#pragma once

#include <networking/PacketHandler.hpp>
#include <common/handlers.hpp>
#include <voyage.pb.h>
#include <entities/EventManager.hpp>

template <typename T>
class ClientCustomHandler : public PacketHandler {
public:
	ClientCustomHandler(EventManager *em) : m_event_manager(em) {

	}	

	void onMessage(RakNet::Packet *p) {
		Message<T> msg(p);
		m_event_manager->emit<T>(msg.getContent());		
	}

private:
	EventManager *m_event_manager;
};
