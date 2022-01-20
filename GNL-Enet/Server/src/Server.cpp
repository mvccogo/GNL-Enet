#include "Server.h"
#include "Packet.h"
#include "NetCommands.h"
using namespace std;

Server::Server() {
	// Allocate some space for World, ServerNetApp classes.
	m_World = std::make_shared<World>(std::shared_ptr<Server>(this));
	m_server_net_app = std::make_shared<ServerNetApp>(1024);
}

Server::~Server() {

}

void Server::OnConnect(uint32_t peerID) {
	ENetPacket* pkt = enet_packet_create("", 0, ENET_PACKET_FLAG_RELIABLE);
	SerializablePacket spkt(pkt);
	spkt << CMD::AcceptConnection;
	SendPacket(peerID, spkt.enet_packet);
}

void Server::OnDisconnect(uint32_t peerID) {

}


void Server::OnPacket(uint32_t peerID, ENetPacket* packet) {
	SerializablePacket spkt(packet);
	uint16_t cmd;
	spkt >> cmd;
	printf("Received command from peer: %d", cmd);
	switch (cmd) {
	case CMD::Ping:
		{
		
		std::cout << "[" << peerID << "]: requested server ping\n";
		long long timeRec;
		auto millisec_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		spkt >> timeRec;
		long long ping = millisec_since_epoch - timeRec;
		std::cout << "Ping (client to server): " << ping << " ms\n";


		auto enet_packet = enet_packet_create("", 0, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
		SerializablePacket spkt_out(enet_packet);
		spkt_out << timeRec;
		spkt_out << CMD::Ping;

		SendPacket(peerID, spkt_out.enet_packet);
		break;
		
		}
	case CMD::SelfEnterWorld:
	{

		std::cout << "[" << peerID << "]: Entered world\n";
		auto ply = AddPlayer(Player(std::shared_ptr<ENetPeer>(GetNetApp()->GetPeerByID(peerID))));
		auto cha = std::make_shared<Character>(ply);
		cha->SetID(peerID);
		cha->SetWorld(m_World);
		m_World->AddChaToWorld(cha->GetID(), *cha);
		auto enet_packet = enet_packet_create("", 0, ENET_PACKET_FLAG_RELIABLE);
		SerializablePacket pkt(enet_packet);

		std::vector<uint32_t> notifylist;
		for (auto i = m_World->m_chaMap.begin(); i != m_World->m_chaMap.end(); i++) {
			pkt << i->second.GetID();
			pkt << i->second.GetPosX();
			pkt << i->second.GetPosY();
			if(i->second.GetID() != cha->GetID())
			notifylist.push_back(i->second.GetID());

		}

		pkt << static_cast<uint32_t>(m_World->m_chaMap.size());
		pkt << cha->GetID();
		pkt << CMD::SelfEnterWorld;
		SendPacket(peerID, pkt.enet_packet);

		

		enet_packet = enet_packet_create("", 0, ENET_PACKET_FLAG_RELIABLE);
		pkt.enet_packet = enet_packet;

		pkt << cha->GetID();
		pkt << cha->GetPosX();
		pkt << cha->GetPosY();
		pkt << CMD::PlyEnterWorld;

		for (auto i = notifylist.begin(); i != notifylist.end(); i++) {
			SendPacket(*i, pkt.enet_packet);
		}
		break;
	}
	case CMD::MovePlayer:
	{
		auto cha = m_World->m_chaMap.find(peerID);
		if (cha != m_World->m_chaMap.end()) {
			// Player ID found (has entered map)
			double_t x, y;
			uint32_t id;
			spkt >> y;
			spkt >> x;
			spkt >> id;

			if (id != cha->second.GetID()) {
				cout << "Error: ID mismatch! Client: " << id << " Server: " << cha->second.GetID() << endl;
			}
			cha->second.SetPos(x, y);
		}
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
	GetNetApp()->GetOutboundQueue()->try_enqueue(out);

}

