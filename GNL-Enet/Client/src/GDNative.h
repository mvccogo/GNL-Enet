#pragma once
#pragma once
#ifndef GDNATIVE_H
#define GDNATIVE_H
#endif

#include <core/Godot.hpp>
#include <gen/Node.hpp>
#include <core/Ref.hpp>
#include "Client.h"
#include <gen/Object.hpp>
#include <enet/enet.h>
#include "Packet.h"
#include <deque>
#include <NetCommands.h>

namespace godot {
	class NetNode : public Node {
		GODOT_CLASS(NetNode, Node);
	public:
		static void _register_methods();

		NetNode();
		~NetNode();

		void _init();
		void _process(float delta);

		void Connect(String host, uint16_t port);
		
		void SendPacket();
		void PreparePacket();
		void FinishReading();
		void AssemblePacket(enet_uint32 flags);

		// Write commands
		void WriteCmd(uint16_t cmdID);
		void WriteShort(uint16_t data);
		void WriteLong(uint32_t data);
		void WriteLongLong(uint64_t data);
		void WriteString(String data);
		void WriteDouble(double_t data);

		// Read Commands

		uint16_t	ReadShort();
		uint32_t	ReadLong();
		uint64_t	ReadLongLong();
		String		ReadString();
		double_t    ReadDouble();

	private:
		ENetHost*								enet_host;
		ENetPeer*								server_peer;
		std::deque<SerializablePacket>			m_pkt_in;
		std::deque<SerializablePacket>			m_pkt_out;
		//SerializablePacket						m_pkt_out_temp;
	};
}