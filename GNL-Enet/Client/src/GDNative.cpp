#include "GDNative.h"
#include "Client.h"

const uint16_t MAX_LEN = 1024 * 5;
using namespace godot;

void NetNode::_register_methods() {
	register_method("_process", &NetNode::_process);
	
	register_method("ConnectToServer", &NetNode::Connect);

	// In-out methodds
	register_method("AssemblePacket", &NetNode::AssemblePacket);
	register_method("FinishReading", &NetNode::FinishReading);

	// Write methods
	register_method("WriteCmd", &NetNode::WriteCmd);
	register_method("WriteShort", &NetNode::WriteShort);
	register_method("WriteLong", &NetNode::WriteLong);
	register_method("WriteLongLong", &NetNode::WriteLongLong);
	register_method("WriteString", &NetNode::WriteString);
	register_method("WriteDouble", &NetNode::WriteDouble);

	// Read methods
	register_method("ReadShort", &NetNode::ReadShort);
	register_method("ReadLong", &NetNode::ReadLong);
	register_method("ReadLongLong", &NetNode::ReadLongLong);
	register_method("ReadString", &NetNode::ReadString);
	register_method("ReadDouble", &NetNode::ReadDouble);

	register_signal<NetNode>((char*)"packet_received", "packet_cmd_id", GODOT_VARIANT_TYPE_INT);
}


NetNode::NetNode()  {
	// Initialize your NativeScript classes in their `_init()` method, not their constructor.The constructor can't access the base class's methods.
}

NetNode::~NetNode() {

	enet_host_destroy(enet_host);
}

void NetNode::_init() {
	enet_host = enet_host_create(NULL, 1, 2, 0, 0);
	if (!enet_host) {
		godot::Godot::print("An error occurred while trying to create an ENet client host.");
	}

}

void NetNode::_process(float delta) {
	ENetEvent event;
	if(enet_host_service(enet_host, &event, 0) >= 0)
	{
		switch (event.type) {
			case ENET_EVENT_TYPE_RECEIVE:
				godot::Godot::print("Received packet from host...\n");

				SerializablePacket spkt(event.packet);
				uint16_t cmd;
				spkt >> cmd;
				m_pkt_in.push_back(spkt);
				emit_signal("packet_received", static_cast<godot_int> (cmd));
		}

		if (!m_pkt_out.empty() && server_peer) {
			// Keep emitting signals until the packet is dealt with.
			auto pkt = m_pkt_out.front().enet_packet;
			enet_peer_send(server_peer, 0, m_pkt_out.front().enet_packet);
			m_pkt_out.pop_front();
		}

	}

}

void NetNode::Connect(String host, uint16_t port) {
	auto a = host.alloc_c_string();
	ENetAddress addr;
	ENetEvent event;
	enet_address_set_host(&addr, host.alloc_c_string());
	addr.port = port;

	server_peer = enet_host_connect(enet_host, &addr, 2, 0);

	if (!server_peer) {
		godot::Godot::print("No available peers for initiating an ENet connection.\n");
	}

	if (enet_host_service(enet_host, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
		godot::Godot::print("Successfully connected over ENet.\n");
	}
	else {
		enet_peer_reset(server_peer);
		godot::Godot::print("Failed to connect over ENet.\n");
	}

}

void NetNode::FinishReading() {
	enet_packet_destroy(m_pkt_in.front().enet_packet);
	m_pkt_in.pop_front();
}

void NetNode::AssemblePacket(enet_uint32 flags) {
	auto enet = enet_packet_create("", 0, flags);
	SerializablePacket spkt(enet);
	m_pkt_out.push_back(spkt);
}

void NetNode::WriteCmd(uint16_t cmdid) {
	SerializablePacket spkt(m_pkt_out.back().enet_packet);
	spkt << static_cast<CMD> (cmdid);
}

void NetNode::WriteShort(uint16_t data) {
	SerializablePacket spkt(m_pkt_out.back().enet_packet);
	spkt << data;
}

void NetNode::WriteLong(uint32_t data) {
	SerializablePacket spkt(m_pkt_out.back().enet_packet);
	spkt << data;
}

void NetNode::WriteLongLong(uint64_t data) {
	SerializablePacket spkt(m_pkt_out.back().enet_packet);
	spkt << data;
}

void NetNode::WriteString(String data) {
	SerializablePacket spkt(m_pkt_out.back().enet_packet);
	size_t oldBody = spkt.enet_packet->dataLength;
	enet_packet_resize(spkt.enet_packet,oldBody + data.length() + 1);
	memset(spkt.enet_packet->data + oldBody, '\0', 1);
	memcpy(spkt.enet_packet->data + oldBody + 1, data.alloc_c_string(), data.length());
}

void NetNode::WriteDouble(double_t data) {
	SerializablePacket spkt(m_pkt_out.back().enet_packet);
	spkt << data;
}


uint16_t NetNode::ReadShort() {
	uint16_t data = 0;
	m_pkt_in.front() >> data;
	return data;
}

uint32_t NetNode::ReadLong() {
	uint32_t data = 0;
	m_pkt_in.front() >> data;
	return data;
}

uint64_t NetNode::ReadLongLong() {
	uint64_t data = 0;
	m_pkt_in.front() >> data;
	return data;
}

String NetNode::ReadString() {
	char data[MAX_LEN]{ 0 };
	size_t oldBody = m_pkt_in.front().enet_packet->dataLength;
	size_t strlen = 0;

	for (auto i = m_pkt_in.front().enet_packet->data; *i; i--) {
		strlen++;
	}
	memcpy(data, m_pkt_in.front().enet_packet->data + oldBody - strlen, strlen);
	enet_packet_resize(m_pkt_in.front().enet_packet, oldBody - strlen - 1);
	return String(data);
}

double_t NetNode::ReadDouble() {
	double_t data = 0.0f;
	m_pkt_in.front() >> data;
	return data;
}