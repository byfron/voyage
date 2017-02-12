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
	_gameEngine.networkManager()->registerHandler(std::make_shared<ServerLoginHandler>
						      (this), {ID_CS_LOGIN_REQUEST});

	_gameEngine.networkManager()->registerHandler(std::make_shared<ServerDataHandler>
						      (this), {ID_CS_USER_MOVEMENT});
	
	

}

void GameServer::stop() {
	google::protobuf::ShutdownProtobufLibrary();
}

PlayerSession::Ptr GameServer::createPlayerSession(RakNet::SystemAddress addr) {

	//make sure the address is not in the map yet
	assert(_playerSessionAddrMap.count(addr) == 0);

	//assign a new id
	int id = _playerSessions.size()+1;

	PlayerSession::Ptr psPtr = PlayerSession::Ptr(
		new PlayerSession(id, addr, GameServer::Ptr(this)));

	_playerSessions.push_back(psPtr);
	_playerSessionAddrMap[addr] = psPtr;

	spd::get("Server")->info() << "Player Session " << id << " successfully created";

	return psPtr;
}

void GameServer::start() {

	spd::get("Server")->info("Starting game server");
	signal(SIGINT, break_loop);
	
	while (!finished_sig && !_finished) {
		_gameEngine.processFrame();
	}

	stop();       
}
