#pragma once

#include <RakPeerInterface.h>
#include <BitStream.h>
#include <StringCompressor.h>
#include <iostream>
#include <MessageIdentifiers.h>

enum MessageId {
	ID_DEBUG = ID_USER_PACKET_ENUM+1,
	ID_LOGIN_MESSAGE,
	ID_DATA_MESSAGE,
	ID_CLIENT_REQUEST_LOGIN,
	ID_CLIENT_SEND_LOGIN_CREDENTIALS
};

/**
 * Generic text message
 */
class Message {

public:

	Message(MessageId id, const std::string data) {
		_id = id;
		_buffer = data.c_str();
		_size = data.size();
	}
	
	Message(RakNet::Packet *p) {
		std::cout << "building message from packet:" << (int)p->data[0] << std::endl;
		RakNet::BitStream bsIn(p->data, p->length, false);
		_id = (MessageId)p->data[0];
		bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
		bsIn.Read(_buffer);
	}

	void toBitStream(RakNet::BitStream & bs) {
		bs.Write((RakNet::MessageID) _id);
		bs.Write(_buffer);
	}

	const char * getData() { return _buffer;}
	int getSize() { return _size;}
	int getId() { return _id; }
	
	friend std::ostream& operator<< (std::ostream& stream, const Message msg);

	typedef std::shared_ptr<Message> Ptr;	
private:

	MessageId _id;
	int _size;
	RakNet::RakString _buffer;
};

inline std::ostream& operator<< (std::ostream& stream, const Message msg) {
	stream << msg._id << "-" << msg._buffer << std::endl;
	return stream;
}
