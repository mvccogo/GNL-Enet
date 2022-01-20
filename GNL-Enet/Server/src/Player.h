#pragma once

#include "Character.h"
#include <memory>
#include <enet/enet.h>

class Character;
class Player {

public: 
	Player(std::shared_ptr<ENetPeer> peer) : m_enet_peer(peer) {};
	~Player() {}

	void SetCharacter(std::shared_ptr<Character> character) { m_character = character; }

private:

	std::shared_ptr<Character>			m_character;
	std::shared_ptr<ENetPeer>			m_enet_peer;


};