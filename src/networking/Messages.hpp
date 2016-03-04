#pragma once

#include <RakPeerInterface.h>
#include <BitStream.h>
#include <StringCompressor.h>
#include <iostream>
#include <MessageIdentifiers.h>
#include <memory>
#include <common/Generic.h>
#include <iostream>
#include <google/protobuf/message.h>

enum MessageId {
	ID_DEBUG = ID_USER_PACKET_ENUM+1,
	ID_LOGIN_MESSAGE,
	ID_DATA_MESSAGE,
	ID_CLIENT_REQUEST_LOGIN,
	ID_CLIENT_SEND_LOGIN_CREDENTIALS
};


/**
 * Wrap of protobuf messages
 */
template <typename T>
class Message {

public:

	Message(MessageId id) {
		_id = id;
		_addr = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
		_size = 0;
	}

	Message(MessageId id, RakNet::BitStream bs) {
		_id = id;
		_addr = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
		_size = 0;
		fromBitStream(bs);
	}

	Message(RakNet::BitStream & bs) {
		fromBitStream(bs);
	}
	
	Message(RakNet::Packet *p) {
		_addr = p->systemAddress;
		RakNet::BitStream bsIn(p->data, p->length, false);
		fromBitStream(bsIn);
	}

	void fromBitStream(RakNet::BitStream & bs) {
		Generic::TempBuffer buffer;
		bs.Read((char*)&_id, (const unsigned int)1);
		bs.IgnoreBytes(sizeof(RakNet::MessageID));
		_size = BITS_TO_BYTES(bs.GetNumberOfUnreadBits());;
		buffer.Ensure(_size);

		bs.Read(buffer);
		_content.ParseFromArray(buffer.data, _size);
	}
	
	void toBitStream(RakNet::BitStream & bs) {

		bs.Write((RakNet::MessageID)_id);
		if (_size > 0) {
			Generic::TempBuffer buffer;
			buffer.Ensure(_size);
			if (_content.SerializeToArray(buffer.data, _size))
			{
				bs.Write((RakNet::MessageID)_id);
			}
			else std::cout << "ERROR" << std::endl;
		}

	}      
	
	int getId() { return _id; }
	RakNet::SystemAddress getAddr() { return _addr; }
	
//	friend std::ostream& operator<< (std::ostream& stream, const Message msg);

	typedef std::shared_ptr<Message> Ptr;	
private:

	MessageId _id;
	RakNet::SystemAddress _addr;
	int _size;
	T _content;  //protobuf message
};

//inline std::ostream& operator<< (std::ostream& stream, const Message msg) {
//	stream << msg._id << "-";// << *msg._content << std::endl;
//	return stream;
//}
