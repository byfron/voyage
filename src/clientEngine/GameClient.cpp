#include "GameClient.hpp"
#include <spdlog/spdlog.h>
#include <thread>

namespace spd = spdlog;

void GameClient::init() {
	
	_netManager.start("127.0.0.1", 1234);
	_netManager.connect();
	_finished = false;
}

void GameClient::clientLoop() {

	std::string cmd;

	while (!_finished) {
		std::cout << ">>";
		std::getline(std::cin, cmd);
	
		_netManager.sendData(ID_DATA_MESSAGE, cmd.c_str());
	}
}

//main loop
void GameClient::start() {

	auto console = spd::stdout_logger_mt("console");
	console->info("Starting game client");


	//TODO: if I don't sleep the message is not sent!!!
	sleep(1);

	_netManager.sendData(ID_LOGIN_MESSAGE);

	//run thread that takes user input
	std::thread p1( [this]() { clientLoop(); } );
	
	while (1) {

		//check for user input
		
		//send messages to server
		//_netManager.sendData(ID_DATA_MESSAGE, cmd.c_str());

		//receive messages from server
		_netManager.receiveData();

		//update world

		//display graphics

		//play sounds
	}		
}
