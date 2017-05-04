#include "GameServer.hpp"
#include <spdlog/spdlog.h>
#include "handlers/ServerLoginHandler.hpp"
#include "handlers/ServerMapHandler.hpp"
#include "handlers/ServerDataHandler.hpp"

#include <signal.h>
#include <common/GameMessages.hpp>
#include "PlayerSession.hpp"
#include <game/World.hpp>

namespace spd = spdlog;

bool finished_sig = false;

void break_loop(int s) {
	finished_sig = true;
}

GameServer::GameServer(int portNum) {

	_gameEngine.init(portNum);


	//TODO refactor to registerNetworkHandlers!!
	_gameEngine.getNetManager().registerHandler(std::make_shared<ServerLoginHandler>
						      (this), {ID_CS_LOGIN_REQUEST});

	_gameEngine.getNetManager().registerHandler(std::make_shared<ServerDataHandler>
						      (this), {ID_CS_USER_ACTION});
}

void GameServer::stop() {
	google::protobuf::ShutdownProtobufLibrary();
}

PlayerSession::Ptr GameServer::createPlayerSession(RakNet::RakNetGUID guid) {

	//make sure the address is not in the map yet
	assert(_playerSessionAddrMap.count(guid) == 0);

	//assign a new id
	int id = _playerSessions.size();

	PlayerSession::Ptr psPtr = PlayerSession::Ptr(
		new PlayerSession(id, guid, getRakNetPeer()));

	_playerSessions.push_back(psPtr);
	_playerSessionAddrMap[guid] = psPtr;

	spd::get("Server")->info() << "Player Session " << id << " with address:" << guid.ToString() << " successfully created";

	return psPtr;
}

void GameServer::broadcastWorldState() {

	for (auto session : _playerSessions) {

		voyage::sc_worldState playerWorldState =
			_gameEngine.computeWorldState(session->getPlayerEntityId()/*player_area*/);

		if (_gameEngine.getNetManager().hasWaitingMsg(session->getPlayerId())) {

			spd::get("Server")->info("has waiting");

			playerWorldState.mutable_player_update()->CopyFrom(_gameEngine.getNetManager().
								     popPlayerStateMsg(session->getPlayerId()));
		}

		// send if we have something ready
		if (playerWorldState.entity_update().size() > 0 ||
		    playerWorldState.has_player_update() ) {

 			spd::get("Server")->info("sending world update");

			session->sendMessage(std::make_shared<Message<voyage::sc_worldState> >
					     (ID_SC_WORLD_STATE, playerWorldState));
		}
	}
}

void GameServer::start() {

	spd::get("Server")->info("Starting game server");
	signal(SIGINT, break_loop);

	while (!finished_sig && !_finished) {

		// wait a bit? which frequency?
		_gameEngine.processFrame();

		// send data to clients
		broadcastWorldState();

		//sleep
		//sleep(1.0f/60.0f);
	}

	stop();
}
