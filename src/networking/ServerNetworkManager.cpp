#include "ServerNetworkManager.hpp"
#include <common/GameMessages.hpp>

ServerNetworkManager::ServerNetworkManager()
{
	_peer = RakNet::RakPeerInterface::GetInstance();

	// DEVEL: create default player-entity map
//	m_map_player_entity[0] = 0;
}
ServerNetworkManager::~ServerNetworkManager()
{
	std::cout << "destroying interface"<< std::endl;
//	assert(false);
	RakNet::RakPeerInterface::DestroyInstance(_peer);
}

void ServerNetworkManager::start(const int port)
{
	RakNet::SocketDescriptor socketDescriptor;
	socketDescriptor.port=(unsigned short) port;
	bool b = _peer->Startup((unsigned short) 600,&socketDescriptor,1)==RakNet::RAKNET_STARTED;
	RakAssert(b);
	_peer->SetMaximumIncomingConnections(600);

	if (not _peer) {
		std::cout << "SERVER raknetpeer null" << std::endl;
		assert(false);
	}
}

unsigned ServerNetworkManager::connectionCount(void) const
{
	unsigned short numberOfSystems;
	_peer->GetConnectionList(0,&numberOfSystems);
	return numberOfSystems;
}
