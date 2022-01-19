#include <NetCommon.h>
#include "ServerNetApp.h"

class World;
class Character;

namespace NetLib {
	class Server {

	public:
		Server(uint16_t port) : m_server_net_app(1000){}

		~Server();

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
		std::list<Player>
		std::shared_ptr<World>        m_World;
		ServerNetApp				  m_server_net_app;


	protected:
		std::mutex						m_mux;
		std::condition_variable			m_cvBlock;
		std::mutex						m_muxBlock;

	};
}
