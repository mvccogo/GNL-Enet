#include "GDNative.h"
#include "Client.h"
#include <enet/enet.h>

const uint16_t MAX_LEN = 1024 * 5;
using namespace godot;

void NetNode::_register_methods() {
	register_method("_process", &NetNode::_process);
	
	register_signal<NetNode>((char*)"packet_received", "packet_cmd_id", GODOT_VARIANT_TYPE_INT);
}


NetNode::NetNode() {
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
	while (enet_host_service(enet_host, &event, 0) > 0)
	{
		switch (event.type) {
			case ENET_EVENT_TYPE_RECEIVE:
				godot::Godot::print("Received packet from host...\n");

				//SerializablePacket pkt;
				//pkt.set_packet_data(event.packet->data, event.packet->dataLength);
				//m_pkt_container.push_front(pkt);
				//enet_packet_destroy(event.packet);
				
		}



	}
	if (!m_pkt_container.empty()) {
		// Keep emitting signals until the packet is dealt with.
		//emit_signal("packet_received", static_cast<int> (m_pkt_container.back().get_packet_cmdid()));
	}


}

void NetNode::Connect(String host, uint16_t port) {
	ENetAddress addr;
	ENetEvent event;
	ENetPeer* peer;
	enet_address_set_host(&addr, host.alloc_c_string());
	addr.port = port;

	peer = enet_host_connect(enet_host, &addr, 2, 0);

	if (!peer) {
		godot::Godot::print("No available peers for initiating an ENet connection.\n");
	}

	if (enet_host_service(enet_host, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
		godot::Godot::print("Successfully connected over ENet.\n");
	}
	else {
		enet_peer_reset(peer);
		godot::Godot::print("Failed to connect over ENet.\n");
	}

}
