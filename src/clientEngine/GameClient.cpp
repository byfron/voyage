#include "GameClient.hpp"
#include <spdlog/spdlog.h>
#include <thread>
#include <signal.h>
#include <graphics/SDL/SDLGraphicsEngine.hpp>

namespace spd = spdlog;

bool gRunning = true;

void exit(int s) {
	gRunning = false;
}

void GameClient::init() {
	
	_netManager.start("127.0.0.1", 1234);
	_netManager.connect();
	_running = true;

	_graphicsEngine = std::make_shared<SDLGraphicsEngine>(GameClient::Ptr(this));
	_graphicsEngine->init();
	
}

void GameClient::clientLoop() {
	
	while (_running && gRunning) {
		
		//_netManager.sendData(ID_DATA_MESSAGE, cmd.c_str());
	}
}

void GameClient::requestQuit() {
	_running = false;
}

//main loop
void GameClient::start() {

	auto console = spd::stdout_logger_mt("console");
	console->info("Starting game client");


	//TODO: if I don't sleep the message is not sent!!!
	sleep(1);

	signal(SIGINT, exit);
	
	_netManager.sendData(ID_LOGIN_MESSAGE);

	//run thread that takes user input
	//std::thread p1( [this]() { clientLoop(); } );

	
	
	while (_running && gRunning) {

		//check for user input
		
		//send messages to server
		//_netManager.sendData(ID_DATA_MESSAGE, cmd.c_str());
		_graphicsEngine->run();
		
		//receive messages from server
		_netManager.receiveData();

		//update world

		//display graphics

		//play sounds
	}

	
}
