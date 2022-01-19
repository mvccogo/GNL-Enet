#pragma once

#include "Character.h"

class Character;
class Player {

public: 
	Player() {};
	~Player() {}

private:

	std::shared_ptr<Character>			m_character;



};