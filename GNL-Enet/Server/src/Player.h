#pragma once

#include "Character.h"
#include <memory>
#include <enet/enet.h>

class Character;
class Player {

public: 
	Player() {};
	~Player() {}

private:

	std::shared_ptr<Character>			m_character;
	std::shared_ptr<ENetPeer>			m_enet_peer;


};