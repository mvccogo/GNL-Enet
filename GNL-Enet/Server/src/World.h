#pragma once
#include <unordered_map>
#include "Character.h"
#include <memory>

class Server;
class Character;

class World {

public:

	World(std::shared_ptr<Server> srv) : m_server(srv) {
		m_chaMap.clear();
	}
	~World() {

	}


	void AddChaToWorld(uint32_t chaID, Character& cha);
	void RemoveChaFromWorld(uint32_t chaID);
	void FlagCharUpdate(Character& cha);
	void FlagMoveUpdate(Character& cha);
	
	std::shared_ptr<Server>					GetServer() { return m_server; }

	std::unordered_map<uint32_t, Character>			m_chaMap;


private:

	std::shared_ptr<Server>			m_server;


};