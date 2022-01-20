#pragma once
#pragma once
#include <string>
//#include "Server.h"
#include "World.h"
#include "Player.h"

class World;
class Player;

class Character {

public:
	Character(std::shared_ptr<Player> player);
	~Character();


	void SetPos(double_t x, double_t y);
	void SetWorld(std::shared_ptr<World> world) { m_World = world; }


	double_t GetPosX() { return pos[0]; }
	double_t GetPosY() { return pos[1]; }



	void SetID(uint32_t id) { m_ID = id; }



	uint32_t GetID() { return m_ID; }
	//void SetTCPConnection(std::shared_ptr<NetLib::TCPConnection<NetLib::CMD>>& ptr) { m_TCPConnection = ptr; }
	//std::shared_ptr<NetLib::TCPConnection<NetLib::CMD>> GetTCPConnection() { return m_TCPConnection; }



private:
	std::shared_ptr<Player>	m_player;
	std::string				m_szName;
	uint32_t				m_ID;
	uint16_t				m_curMapID;
	double_t				pos[2];
	std::shared_ptr<World>  m_World;
};