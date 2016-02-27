#include "ServerNetworkManager.hpp"
#include "ServerLoginHandler.hpp"
#include "ServerDataHandler.hpp"

ServerNetworkManager::ServerNetworkManager()
{
	_peer = RakNet::RakPeerInterface::GetInstance();
}
ServerNetworkManager::~ServerNetworkManager()
{
	RakNet::RakPeerInterface::DestroyInstance(_peer);
}

void ServerNetworkManager::registerHandler(PacketHandlerPtr h) {
	_handlers[h->getType()] = h;
}

void ServerNetworkManager::start()
{
	RakNet::SocketDescriptor socketDescriptor;
	socketDescriptor.port=(unsigned short) SERVER_PORT;
	bool b = _peer->Startup((unsigned short) 600,&socketDescriptor,1)==RakNet::RAKNET_STARTED;
	RakAssert(b);
	_peer->SetMaximumIncomingConnections(600);
	
	registerHandler(std::make_shared<ServerLoginHandler>());
	registerHandler(std::make_shared<ServerDataHandler>());
	
}


void ServerNetworkManager::receiveData()
{
	RakNet::Packet *p = _peer->Receive();

	while(p) {
		switch(p->data[0]) {
		case ID_LOGIN_MESSAGE:
			_handlers[SERVER_LOGIN_HANDLER]->onPacket(std::make_shared<LoginMessage>(p));
			break;

		case ID_DATA_MESSAGE:
			_handlers[SERVER_DATA_HANDLER]->onPacket(std::make_shared<DataMessage>(p));
			break;			
		};
		
		_peer->DeallocatePacket(p);
		p = _peer->Receive();
	}
}
