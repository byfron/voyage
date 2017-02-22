#pragma once

#include <queue>
#include <voyage.pb.h>

class NetworkMessagePool {

public:
	NetworkMessagePool() {

	}

	void pushEntityUpdateMsg(const voyage::sc_entityState & msg) {
		m_entity_update_queue[msg.entityid()].push(msg);
	}

	voyage::sc_entityState popEntityUpdateMsg(uint32_t id) {
		voyage::sc_entityState msg = m_entity_update_queue[id].front();
		m_entity_update_queue[id].pop();
		return msg;
	}

	void pushPlayerUpdateMsg(const voyage::sc_playerState & msg) {
		m_player_update_queue.push(msg);
	}
	
	voyage::sc_playerState popPlayerUpdateMsg() {
		voyage::sc_playerState msg = m_player_update_queue.front();
		m_player_update_queue.pop();
		return msg;
	}

	bool hasPlayerMsg() {
		return m_player_update_queue.size() > 0;
	}

	bool hasEntityMsg(uint32_t id) {
		return m_entity_update_queue[id].size() > 0;
	}

private:

	std::queue<voyage::sc_playerState> m_player_update_queue;
	std::map<uint32_t, std::queue<voyage::sc_entityState> > m_entity_update_queue;

};
