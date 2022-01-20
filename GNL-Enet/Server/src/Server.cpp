#include "Server.h"
#include "Packet.h"
#include "NetCommands.h"
using namespace std;

Server::Server(uint16_t port) {
	// Allocate some space for World, ServerNetApp classes.
	m_World = std::make_shared<World>();
	m_server_net_app = std::make_shared<ServerNetApp>(1024);
}

Server::~Server() {

}

void Server::OnPacket(uint32_t peerID, ENetPacket* packet) {
	SerializablePacket spkt(packet);
	uint16_t cmd;
	spkt >> cmd;
	switch (cmd) {
	case CMD::Ping:
		{
		
		std::cout << "[" << peerID << "]: requested server ping\n";
		long long timeRec;
		auto millisec_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		spkt >> timeRec;
		long long ping = millisec_since_epoch - timeRec;
		std::cout << "Ping (client to server): " << ping << " ms\n";


		auto enet_packet = enet_packet_create("", 1, ENET_PACKET_FLAG_RELIABLE);
		SerializablePacket spkt_out(enet_packet);
		spkt_out << timeRec;
		spkt_out << CMD::Ping;

		SendPacket(peerID, spkt_out.enet_packet);
		break;
		
		}
	}
	enet_packet_destroy(packet);
}

void Server::SendPacket(uint32_t peerID, ENetPacket* packet) {
	ServerNetApp::OutboundPkt out;
	SerializablePacket spkt_out(packet);
	out.peer = GetNetApp()->GetPeerByID(peerID);
	out.packet = spkt_out.enet_packet;
	std::cout << spkt_out.enet_packet << "\n";
	GetNetApp()->GetOutboundQueue()->try_enqueue(out);

}

