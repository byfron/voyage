#include "GameServer.hpp"
#include <spdlog/spdlog.h>
#include "handlers/ServerLoginHandler.hpp"
#include "handlers/ServerDataHandler.hpp"

namespace spd = spdlog;

void GameServer::init(int port) {
	_netManager.start(port);
	_finished = false;

	_netManager.registerHandler(std::make_shared<ServerLoginHandler>(GameServer::Ptr(this)),
				    {ID_LOGIN_MESSAGE, ID_CLIENT_REQUEST_LOGIN});
	
	_netManager.registerHandler(std::make_shared<ServerDataHandler>(),
				    {ID_DATA_MESSAGE});	
	
}

void GameServer::start() {
	auto console = spd::stdout_logger_mt("Server");
	console->info("Starting game server");

	while (!_finished) {

		//check for client commands

		_netManager.receiveData();
		
		//    run AI
//    move all entities

//    send updates about the game to the clients
		
	}
}
