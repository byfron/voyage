#include "ClientNetworkManager.hpp"
#include <common/GameMessages.hpp>
#include <voyage.pb.h>

//TODO: we can move up the hirearchy plenty of common functionality
//of this and the servernetworkmanager

ClientNetworkManager::ClientNetworkManager()
{
	_peer = RakNet::RakPeerInterface::GetInstance();
}
ClientNetworkManager::~ClientNetworkManager()
{
	RakNet::RakPeerInterface::DestroyInstance(_peer);
}

void ClientNetworkManager::start(const char *address, const int port)
{
	_remoteIPAddress = address;
	_port = port;
	RakNet::SocketDescriptor socketDescriptor;
	socketDescriptor.port=0;
	RakNet::StartupResult b = _peer->Startup(1,&socketDescriptor,1);
	RakAssert(b==RAKNET_STARTED);
	_isConnected=false;		
}

void ClientNetworkManager::connect()
{
	bool b;
	b = _peer->Connect(_remoteIPAddress, (unsigned short) _port, 0, 0, 0)==RakNet::CONNECTION_ATTEMPT_STARTED;
	if (b==false)
	{
		printf("Client connect call failed!\n");
	}

	_isConnected = true;
		
}
void ClientNetworkManager::disconnect(void)
{
	_peer->CloseConnection(_peer->GetSystemAddressFromIndex(0),true,0);
	_isConnected=false;
}

// template <typename T>
// void ClientNetworkManager::sendData(RakNet::MessageID id, const T & content) {

// 	RakNet::BitStream bsOut;

// 	Message<T>::Ptr m = std::make_shared<T>(id, content);
// 	_peer->Send(&bsOut, HIGH_PRIORITY,RELIABLE_ORDERED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
	
// 	switch(id) {

// 	case ID_CS_LOGIN_REQUEST:			
// 		Message<voyage::cs_LoginRequest>::Ptr m =
// 			std::make_shared< Message<voyage::cs_LoginRequest> >(id);
// 		m->toBitStream(bsOut);
// 		break;
			
// 	case ID_CS_USER_MOVEMENT:
// 		Message<voyage::cs_userMovement>::Ptr m =
// 			std::make_shared< Message<voyage::cs_userMovement> >(id);
// 		m->toBitStream(bsOut);
// 		break;

// 	};			
// }
