#pragma once
#pragma once
#include <stdint.h>
#include <vector>
#include <enet/enet.h>

class SerializablePacket {
	// A wrapper around ENetPacket
public:
	SerializablePacket(ENetPacket* packet) : enet_packet(packet) {};
	~SerializablePacket() {
	
	};

	template <typename DataType>
	friend SerializablePacket& operator << (SerializablePacket& pkt, const DataType& data) {

		// Check that the type of the data being pushed is trivially copyable
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

		// Cache current size of vector, as this will be the point we insert the data
		size_t i = pkt.enet_packet->dataLength;
		// Resize the packet by the size of the data being pushed
		enet_packet_resize(pkt.enet_packet, pkt.enet_packet->dataLength + sizeof(DataType));

		// Physically copy the data into the newly allocated vector space
		std::memcpy(pkt.enet_packet->data + i, &data, sizeof(DataType));

		// Return the target message so it can be "chained"
		return pkt;
	}

	template<typename DataType>
	friend SerializablePacket& operator >> (SerializablePacket& pkt, DataType& data) {
		// Check that the type of the data being pushed is trivially copyable
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

		// Cache the location towards the end of the vector where the pulled data starts
		size_t i = pkt.enet_packet->dataLength - sizeof(DataType);
		if (i < 0) {
			printf("Corrupted packet, ignoring output operation");
			return pkt;
		};
		// Physically copy the data from the vector into the user variable
		std::memcpy(&data, pkt.enet_packet->data + i, sizeof(DataType));

		// Shrink the vector to remove read bytes, and reset end position
		enet_packet_resize(pkt.enet_packet, i);

		// Return the target message so it can be "chained"
		return pkt;
	}

	ENetPacket*				enet_packet;



};