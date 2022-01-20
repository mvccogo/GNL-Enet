#include <NetCommon.h>
#include "ServerNetApp.h"
#include "Player.h"

class World;
class Character;
class Player;
class ServerNetApp;

class Server {
	public:

		Server(uint16_t port);
		~Server();

		void Start() {
			m_server_net_app->Start(2004);
			m_server_net_app->Run();
		}

		std::shared_ptr<ServerNetApp> GetNetApp() { return m_server_net_app; }

		void OnPacket(uint32_t peerID, ENetPacket packet);

		void SendPacket(uint32_t peerID, ENetPacket packet);


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


};

