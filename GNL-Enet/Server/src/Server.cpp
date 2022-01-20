#include "Server.h"

using namespace std;

Server::Server(uint16_t port) {
	// Allocate some space for World, ServerNetApp classes.
	m_World = std::make_shared<World>();
	m_server_net_app = std::make_shared<ServerNetApp>(1024);
}

Server::~Server() {

}
void Server::OnPacket(uint32_t peerID, ENetPacket packet) {

	uint16_t cmd_id = (uint16_t)*packet.data;
	switch (cmd_id) {




	}

}

void Server::SendPacket(uint32_t peerID, ENetPacket packet) {
	ServerNetApp::OutboundPkt out;
	out.packet = packet;
	out.peer = *GetNetApp()->GetPeerByID(peerID);

	GetNetApp()->GetOutboundQueue()->try_enqueue(out);


}

