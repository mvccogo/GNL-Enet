#include "World.h"
#include "Character.h"
#include "NetCommands.h"
#include "enet/enet.h"
#include "Packet.h"
#include "Server.h"

using namespace std;
class Server;
//
//World::World() {
//
//
//
//}
//World::~World() {
//
//
//}

void World::AddChaToWorld(uint32_t chaID, Character& cha) {
	m_chaMap.insert(std::make_pair(chaID, cha));
}

void World::RemoveChaFromWorld(uint32_t chaID) {
	m_chaMap.erase(chaID);
}

void World::FlagMoveUpdate(Character& cha) {
	auto enet_packet = enet_packet_create("", 0, ENET_PACKET_FLAG_RELIABLE);
	SerializablePacket spkt(enet_packet);

	spkt << cha.GetID();
	spkt << cha.GetVelX();
	spkt << cha.GetVelY();
	spkt << CMD::MovePlayer;
	for (auto i = m_chaMap.begin(); i != m_chaMap.end(); i++) {
		if (i->second.GetID() == cha.GetID()) continue;
		GetServer()->SendPacket(i->second.GetID(), spkt.enet_packet);
	}

}

void World::Run(unsigned long long delta) {

	for (auto i = m_chaMap.begin(); i != m_chaMap.end(); i++) {
		if (i->second.GetVelX() || i->second.GetVelY()) {
			double_t pos_increment[2] = { delta * i->second.GetVelX(), delta * i->second.GetVelY() };
			i->second.SetPos(i->second.GetPosX() + pos_increment[0], i->second.GetPosY() + pos_increment[1]);
		}
	}

}