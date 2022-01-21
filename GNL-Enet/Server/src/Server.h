#include <NetCommon.h>
#include "ServerNetApp.h"
#include "Player.h"

class World;
class Character;
class Player;
class ServerNetApp;

class Server {
public:

	Server();
	~Server();

	void Start(uint16_t port) {
		m_server_net_app->Start(port);
		m_server_net_app->Run();
	}

	std::shared_ptr<ServerNetApp>	GetNetApp() { return m_server_net_app; }
	std::shared_ptr<Player>			AddPlayer(Player&& ply) { m_players.push_back(ply); return std::shared_ptr<Player>(&m_players.back()); }


	void GameLoop(unsigned long long curTick);

	void OnPacket(uint32_t peerID, ENetPacket* packet);
	void OnConnect(uint32_t peerID);
	void OnDisconnect(uint32_t peerID);

	void SendPacket(uint32_t peerID, ENetPacket* packet);


protected:
	/*	virtual bool OnClientConnect(std::shared_ptr<TCPConnection<CMD>>& client) override {
			Packet<CMD> pkt;
			pkt.header.cmdID = CMD::AcceptTCPConnection;
			SendToClient(client, pkt);
			return true;
		}
		virtual void OnClientDisconnect(std::shared_ptr<TCPConnection<CMD>>& client) override;
		virtual void OnPacket(std::shared_ptr<TCPConnection<CMD>>& client, Packet<CMD>& pkt) override;
	*/


private:
	std::list<Player>				m_players;
	std::shared_ptr<World>			m_World;
	std::shared_ptr<ServerNetApp>	m_server_net_app;

public:
	unsigned long long				m_lastTick;


};

