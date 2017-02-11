#include "ServerNetworkManager.hpp"
#include <common/GameMessages.hpp>

ServerNetworkManager::ServerNetworkManager()
{
	_peer = RakNet::RakPeerInterface::GetInstance();
}
ServerNetworkManager::~ServerNetworkManager()
{
	RakNet::RakPeerInterface::DestroyInstance(_peer);
}

void ServerNetworkManager::start(const int port)
{
	RakNet::SocketDescriptor socketDescriptor;
	socketDescriptor.port=(unsigned short) port;
	bool b = _peer->Startup((unsigned short) 600,&socketDescriptor,1)==RakNet::RAKNET_STARTED;
	RakAssert(b);
	_peer->SetMaximumIncomingConnections(600);       
}

unsigned ServerNetworkManager::connectionCount(void) const
{
	unsigned short numberOfSystems;
	_peer->GetConnectionList(0,&numberOfSystems);
	return numberOfSystems;
}
