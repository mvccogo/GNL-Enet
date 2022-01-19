#pragma once
#include <NetCommon.h>
#include <enet/enet.h>
#include <ConcurrentQueue.h>

class ServerNetApp {
	// This class will deal with all networking for now

	public: 
		ServerNetApp(size_t initialQueueSize) : m_event_queue(initialQueueSize) {}

		~ServerNetApp() {}

		bool Start(uint16_t port);
		void Run();

		void Stop();


private:

		std::deque<ENetPeer>							m_peers;
		moodycamel::ConcurrentQueue<ENetEvent>			m_inbound_queue;
		moodycamel::ConcurrentQueue<ENetEvent>			m_outbound_queue;
		ENetAddress										m_enet_addr;
		std::shared_ptr<ENetHost>						m_enet_host;




};