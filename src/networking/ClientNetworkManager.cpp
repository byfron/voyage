#include "ClientNetworkManager.hpp"

ClientNetworkManager::ClientNetworkManager()
{
	_peer = RakNet::RakPeerInterface::GetInstance();
}
ClientNetworkManager::~ClientNetworkManager()
{
	RakNet::RakPeerInterface::DestroyInstance(_peer);
}

void ClientNetworkManager::registerHandler(PacketHandler::Ptr h) {
	_handlers[h->getId()] = h;
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


void ClientNetworkManager::sendData(MessageId id) {
	
	Message::Ptr m = std::make_shared<Message>(id);
	RakNet::BitStream bsOut;
	m->toBitStream(bsOut);
	_peer->Send(&bsOut, HIGH_PRIORITY,RELIABLE_ORDERED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
}

void ClientNetworkManager::sendData(MessageId id, const char *data) {
	
	Message::Ptr m = std::make_shared<Message>(id, data);
	RakNet::BitStream bsOut;
	m->toBitStream(bsOut);
	_peer->Send(&bsOut, HIGH_PRIORITY,RELIABLE_ORDERED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
}

void ClientNetworkManager::receiveData()
{
	Packet *p;
	for (p=_peer->Receive(); p; _peer->DeallocatePacket(p), p=_peer->Receive())
	{
		Message::Ptr m = std::make_shared<Message>(p);

		std::cout << "receiving something:" << *m << std::endl;
		
		switch(m->getId()) {		
					
			/*case ID_LOGIN_MESSAGE:
			_handlers[CLIENT_LOGIN_HANDLER]->onMessage(m);
			break;

		case ID_DATA_MESSAGE:
			_handlers[CLIENT_DATA_HANDLER]->onMessage(m);
			break;			
		};
			*/
		};
	}
	
}
