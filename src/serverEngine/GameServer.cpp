#include "GameServer.hpp"
#include <spdlog/spdlog.h>
#include "handlers/ServerLoginHandler.hpp"
#include "handlers/ServerDataHandler.hpp"
#include <signal.h>
#include <common/GameMessages.hpp>

namespace spd = spdlog;

bool finished_sig = false;

void break_loop(int s) {
	finished_sig = true;
}

GameServer::GameServer(int portNum, int poolSize) :
	_dbPoolPtr(std::make_shared<DatabaseConnectionPool>(poolSize)) {
	_datasetName = DB_DIR DB_NAME;
	_init(portNum);
}

void GameServer::_init(int port) {

	//initialize server
	_netManagerPtr = std::make_shared<ServerNetworkManager>();
	_netManagerPtr->start(port);
	_finished = false;

	//initialize database pool
	_dbPoolPtr->fillPool(_datasetName);

	//initialize scripting engine
	_scriptEnginePtr = std::make_shared<ScriptEngine>();
	_scriptEnginePtr->init(_dbPoolPtr);
	
	//register managers
	_netManagerPtr->registerHandler(std::make_shared<ServerLoginHandler>(GameServer::Ptr(this)),
				    {ID_CS_LOGIN_REQUEST});
	
//	_netManagerPtr->registerHandler(std::make_shared<ServerDataHandler>(GameServer::Ptr(this)),
//				    {ID_DATA_MESSAGE});		
}

void GameServer::stop() {
	_dbPoolPtr->drainPool();
	_scriptEnginePtr->finalize();

	google::protobuf::ShutdownProtobufLibrary();

}

int GameServer::createPlayerSession(RakNet::SystemAddress addr) {

	//make sure the address is not in the map yet
	assert(_playerSessionAddrMap.count(addr) == 0);

	//assign a new id
	int id = _playerSessions.size()+1;

	PlayerSession::Ptr psPtr = PlayerSession::Ptr(
		new PlayerSession(id, addr, GameServer::Ptr(this)));

	_playerSessions.push_back(psPtr);
	_playerSessionAddrMap[addr] = psPtr;

	spd::get("Server")->info() << "Player Session " << id << " successfully created";
	
	return 0;
}

void GameServer::start() {

	spd::get("Server")->info("Starting game server");
	signal(SIGINT, break_loop);
	
	while (!finished_sig && !_finished) {

		//check for client commands

		_netManagerPtr->receiveData();
		
		//    run AI
//    move all entities

//    send updates about the game to the clients
		
	}

	stop();
	
}
