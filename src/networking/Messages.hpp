#pragma once

#include <RakPeerInterface.h>
#include <iostream>

enum Messages {
	ID_LOGIN_MESSAGE,
	ID_DATA_MESSAGE
};

class LoginMessage{
public:
	LoginMessage(RakNet::Packet *p) {
		std::cout << "processing login" << std::endl;
	};
	
private:
	
};


class DataMessage {
public:
	DataMessage(RakNet::Packet *p) {
		std::cout << "processing data" << std::endl;
	};

};
