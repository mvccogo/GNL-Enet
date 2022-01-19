#pragma once
#include <stdint.h>

class Packet {
public:
	Packet();
	~Packet();

	uint32_t get_packet_len() const {return static_cast<uint32_t>(body.size()) }


private:

	uint16_t cmdID;
	uint32_t size;


};