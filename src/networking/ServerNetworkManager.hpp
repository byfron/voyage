#ifndef __VOYAGE_SERVER_HPP__
#define __VOYAGE_SERVER_HPP__

#include <RakPeerInterface.h>
#include <map>
#include <list>
#include "PacketHandler.hpp"
#include "Session.hpp"
#include "NetworkManager.hpp"
#include <spdlog/spdlog.h>
#include "voyage.pb.h"

namespace spd = spdlog;

#define SERVER_PORT 1234

class ServerNetworkManager : public NetworkManager
{
public:
	ServerNetworkManager();
	~ServerNetworkManager();	
	void start(const int port);
	void stop();

	unsigned connectionCount(void) const;

	typedef std::shared_ptr<ServerNetworkManager> Ptr;

	uint32_t getPlayerEntityId(uint32_t player_id) {
		if (m_map_player_entity.count(player_id) == 0) {
			spd::get("Server")->warn() <<
				"Accessing player map with non existent player id " << player_id;
			return -1;
		}
		return m_map_player_entity[player_id];
	}

	void pushActionSeq(uint32_t player_id, uint32_t action_seq) {
		// TODO: shall we keep a list?
		m_map_player_actionseq[player_id] = action_seq;
	}

	void pushPlayerStateMsg(uint32_t player_id, voyage::sc_playerState ps) {
		m_player_response_messages[player_id] = ps;
	}

	voyage::sc_playerState getPlayerStateMsg(uint32_t player_id) {
		assert(hasWaitingMsg(player_id));
		return m_player_response_messages[player_id];
	}

	bool hasWaitingMsg(uint32_t player_id) {
		return m_player_response_messages.count(player_id) > 0;
	}
	
private:
	bool _server_running;
	
	// keep a map of which entities are controled by which players
	std::map<uint32_t, uint32_t> m_map_player_entity;

	// keep a map of the last action seq. received from each player
	std::map<uint32_t, uint32_t> m_map_player_actionseq;

	// map of response messages for the different sessions
	std::map<uint32_t, voyage::sc_playerState> m_player_response_messages;
};

#endif
