#include <serverEngine/GameServer.hpp>

int main()
{	
	GameServer server;
	server.init(1234);
	server.start();   
}
