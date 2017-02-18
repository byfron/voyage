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

	return;

	// TODO: handle all this in the loginHandler

	printf("CONNECTING..\n");
	
	Packet *p = _peer->Receive();
	while (p)
	{
		switch (p->data[0])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
			printf("ID_CONNECTION_REQUEST_ACCEPTED\n");

			sendCustomLoginRequest();
			
			_isConnected=true;
			break;
		case ID_CONNECTION_ATTEMPT_FAILED:
			printf("Client Error: ID_CONNECTION_ATTEMPT_FAILED\n");
			_isConnected=false;
			break;
		case ID_ALREADY_CONNECTED:
			printf("Client Error: ID_ALREADY_CONNECTED\n");
			break;
		case ID_CONNECTION_BANNED:
			printf("Client Error: ID_CONNECTION_BANNED\n");
			break;
		case ID_INVALID_PASSWORD:
			printf("Client Error: ID_INVALID_PASSWORD\n");
			break;
		case ID_INCOMPATIBLE_PROTOCOL_VERSION:
			printf("Client Error: ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			printf("Client Error: ID_NO_FREE_INCOMING_CONNECTIONS\n");
			_isConnected=false;
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			//printf("ID_DISCONNECTION_NOTIFICATION\n");
			_isConnected=false;
			break;
		case ID_CONNECTION_LOST:
			printf("Client Error: ID_CONNECTION_LOST\n");
			_isConnected=false;
			break;
		}
		_peer->DeallocatePacket(p);
		p = _peer->Receive();
			
	}

	printf("DONE\n");
		
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


void ClientNetworkManager::sendCustomLoginRequest() {
	// send login message
	voyage::cs_loginRequest logreq;
	logreq.set_username("byfron");
	logreq.set_password("1234");
	sendData<voyage::cs_loginRequest>(ID_CS_LOGIN_REQUEST, logreq);

}
