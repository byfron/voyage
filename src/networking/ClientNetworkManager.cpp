#include "ClientNetworkManager.hpp"
#include "ClientLoginHandler.hpp"
#include "ClientDataHandler.hpp"

ClientNetworkManager::ClientNetworkManager()
{
	_peer = RakNet::RakPeerInterface::GetInstance();
}
ClientNetworkManager::~ClientNetworkManager()
{
	RakNet::RakPeerInterface::DestroyInstance(_peer);
}

void ClientNetworkManager::registerHandler(PacketHandlerPtr h) {
	_handlers[h->getType()] = h;
}

void ClientNetworkManager::start()
{
	RakNet::SocketDescriptor socketDescriptor;
	socketDescriptor.port=0;
	RakNet::StartupResult b = _peer->Startup(1,&socketDescriptor,1);
	RakAssert(b==RAKNET_STARTED);
	_isConnected=false;

	registerHandler(std::make_shared<ClientLoginHandler>());
	registerHandler(std::make_shared<ClientDataHandler>());
	
}

void ClientNetworkManager::connect()
{
	bool b;
	b = _peer->Connect(_remoteIPAddress, (unsigned short) SERVER_PORT, 0, 0, 0)==RakNet::CONNECTION_ATTEMPT_STARTED;
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

void ClientNetworkManager::receiveData()
{
	Packet *p = _peer->Receive();

	while(p) {
		switch(p->data[0]) {
		case ID_LOGIN_MESSAGE:
			_handlers[CLIENT_LOGIN_HANDLER]->onPacket(std::make_shared<LoginMessage>(p));
			break;

		case ID_DATA_MESSAGE:
			_handlers[CLIENT_DATA_HANDLER]->onPacket(std::make_shared<DataMessage>(p));
			break;			
		};
		
		_peer->DeallocatePacket(p);
		p = _peer->Receive();
	}
}
