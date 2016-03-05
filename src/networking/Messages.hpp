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

/**
 * Wrap of protobuf messages
 */
template <typename T>
class Message {

public:

	Message(RakNet::MessageID id, T content) {
		_id = id;
		_size = content.ByteSize();
		_content = content;
		_addr = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
	}
	
	Message(RakNet::MessageID id) {
		_id = id;
		_addr = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
		_size = 0;
	}

	Message(RakNet::MessageID id, RakNet::BitStream bs) {
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
		_size = BITS_TO_BYTES(bs.GetNumberOfUnreadBits());;
		buffer.Ensure(_size);
		bs.Read(buffer.data, _size);		
		_content.ParseFromArray(buffer.data, _size);
	}
	
	void toBitStream(RakNet::BitStream & bs) {

		bs.Write((RakNet::MessageID)_id);
		if (_size > 0) {
			Generic::TempBuffer buffer;
			buffer.Ensure(_size);
			if (_content.SerializeToArray(buffer.data, buffer.size))
			{
				bs.Write(buffer.data, buffer.size);
			}
			else std::cout << "ERROR" << std::endl;
		}
	}      
	
	int getId() { return _id; }
	RakNet::SystemAddress getAddr() { return _addr; }
	T getContent() { return _content; }

	template <typename J>
	friend std::ostream& operator<< (std::ostream& stream, const Message<J> msg);

	typedef typename std::shared_ptr< Message<T> > Ptr;	
private:

	RakNet::MessageID _id;
	RakNet::SystemAddress _addr;
	int _size;
	T _content;  //protobuf message
};

template <typename T>
inline std::ostream& operator<< (std::ostream& stream, const Message<T> msg) {
	stream << (int)msg._id << "-";
	msg._content.SerializeToOstream(&stream);
	return stream;
}
