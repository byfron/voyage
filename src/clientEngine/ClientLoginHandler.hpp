#pragma once

#include "PacketHandler.hpp"

class ClientLoginHandler : public PacketHandler {

public:

	void onMessage(Message::Ptr m) {	       
		std::cout << *m << std::endl;
	}
};
