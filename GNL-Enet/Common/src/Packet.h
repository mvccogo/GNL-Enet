#pragma once
#pragma once
#include <stdint.h>
#include <vector>

class Packet {
public:
	Packet() {};
	~Packet() {};

	uint32_t get_packet_len() const { return static_cast<uint32_t>(body.size()); }

	void	set_packet_data(uint8_t* dataptr, size_t datalen) {
		// This has terrible performance, use the packet from ENet directly!
		cmdID = (uint16_t)*dataptr;
		body.resize(datalen - sizeof(uint16_t));
		std::memcpy(&body, dataptr + sizeof(uint16_t), datalen - sizeof(uint16_t));
		size = datalen - sizeof(uint16_t);
	}

	uint16_t get_packet_cmdid() const { return cmdID; }

	template <typename DataType>
	friend Packet& operator << (Packet& pkt, const DataType& data) {

		// Check that the type of the data being pushed is trivially copyable
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

		// Cache current size of vector, as this will be the point we insert the data
		size_t i = pkt.body.size();
		// Resize the vector by the size of the data being pushed
		pkt.body.resize(pkt.body.size() + sizeof(DataType));

		// Physically copy the data into the newly allocated vector space
		memcpy(pkt.body.data() + i, &data, sizeof(DataType));

		// Recalculate the message size
		pkt.size = pkt.get_packet_len();

		// Return the target message so it can be "chained"
		return pkt;
	}

	template<typename DataType>
	friend Packet& operator >> (Packet& pkt, DataType& data) {
		// Check that the type of the data being pushed is trivially copyable
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");


		// Cache the location towards the end of the vector where the pulled data starts
		size_t i = pkt.body.size() - sizeof(DataType);

		// Physically copy the data from the vector into the user variable
		memcpy(&data, pkt.body.data() + i, sizeof(DataType));

		// Shrink the vector to remove read bytes, and reset end position
		pkt.body.resize(i);

		// Recalculate the message size
		pkt.size = pkt.get_packet_len();

		// Return the target message so it can be "chained"
		return pkt;
	}



private:

	uint16_t				cmdID;
	uint32_t				size;
	std::vector<uint8_t>	body;



};