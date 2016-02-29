#include "ServerNetworkManager.hpp"

ServerNetworkManager::ServerNetworkManager()
{
	_peer = RakNet::RakPeerInterface::GetInstance();
}
ServerNetworkManager::~ServerNetworkManager()
{
	RakNet::RakPeerInterface::DestroyInstance(_peer);
}

void ServerNetworkManager::registerHandler(PacketHandler::Ptr h, const std::list<int> & list) {

	h->isRegistered = true;

	for (auto m : list) {
		_msgHandlersMap[m] = h->getId();
	}
	
	_handlers[h->getId()] = h;	
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

void ServerNetworkManager::receiveData()
{
	RakNet::Packet *p;
	for (p=_peer->Receive(); p; _peer->DeallocatePacket(p), p=_peer->Receive())
	{
		Message::Ptr m = std::make_shared<Message>(p);

		if (_msgHandlersMap.count(m->getId()))
			_handlers[_msgHandlersMap[m->getId()]]->onMessage(m);
	}
}
