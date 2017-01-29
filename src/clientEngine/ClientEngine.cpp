#include "ClientEngine.hpp"
#include <spdlog/spdlog.h>
#include <thread>
#include <signal.h>
#include <common/GameMessages.hpp>
#include "handlers/ClientLoginHandler.hpp"
#include "handlers/ClientMapHandler.hpp"
#include "voyage.pb.h"
#include <iostream>

namespace spd = spdlog;

bool gRunning = true;

// void exit(int s) {
// 	gRunning = false;
// }

void ClientEngine::init() {

	std::cout << "Initializing ClientEngine";

	_netManager = std::make_shared<ClientNetworkManager>();
	_netManager->start("127.0.0.1", 1234);
	_netManager->connect();
	_running = true;

//	_tileMap = std::make_shared<ClientTileMap>();

	_systemManager = std::make_shared<SystemManager>(_entityManager,
							 _eventManager);

	_registerHandlers();

	// create world
	m_world = std::make_shared<World>();


	//_graphicsEngine = std::make_shared<SDLGraphicsEngine>(ClientEngine::Ptr(this));
	//_graphicsEngine->init();


	// initialise event manager

	// initialise entity manager (will have graphics/audio components here,
	// but all the logic is in the server)

}

void ClientEngine::_registerHandlers() {

	_netManager->registerHandler(std::make_shared<ClientLoginHandler>(ClientEngine::Ptr(this)),
				     {ID_SC_LOGIN_ACCEPTED});

	_netManager->registerHandler(std::make_shared<ClientMapHandler>(ClientEngine::Ptr(this)),
				     {ID_SC_REGION_DATA});
}

void ClientEngine::requestQuit() {
	_running = false;
}


void ClientEngine::processFrame() {

	//check for user input

	//update local entities

	//send messages to server

	//receive messages from server
	_netManager->receiveData();

	//update world

	//display graphics
	//_graphicsEngine->run();

	//play sounds
}

// //main loop\
// void ClientEngine::start() {

// 	//auto console = spd::stdout_logger_mt("console");
// 	//console->info("Starting game client");

// 	//TODO: if I don't sleep the message is not sent!!!
// 	// sleep(1);

// 	// signal(SIGINT, exit);

// 	// voyage::cs_loginRequest msg;
// 	// msg.set_username("jose");
// 	// msg.set_password("1234");

// //	_netManager->sendData<voyage::cs_loginRequest>(ID_CS_LOGIN_REQUEST, msg);

// }
