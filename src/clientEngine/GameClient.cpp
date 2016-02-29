#include "GameClient.hpp"
#include <spdlog/spdlog.h>

namespace spd = spdlog;

void GameClient::init() {
	
	_netManager.start("127.0.0.1", 1234);
	_netManager.connect();
	_finished = false;
}

//main loop
void GameClient::start() {

	auto console = spd::stdout_logger_mt("console");
	console->info("Starting game client");

	std::string cmd;
	
	while (!_finished) {

		//check for user input
		std::cout << ">>";
		std::getline(std::cin, cmd);
		
		//send messages to server
		_netManager.sendData(ID_LOGIN_MESSAGE, cmd.c_str());

		//receive messages from server
		_netManager.receiveData();

		//update world

		//display graphics

		//play sounds
	}		
}
