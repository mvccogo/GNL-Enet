#pragma once
#include <NetCommon.h>
#include <enet/enet.h>
#include <ConcurrentQueue.h>
#include <unordered_map>
#include <mutex>

class ServerNetApp {
	// This class will deal with all networking for now

	public: 

		struct OutboundPkt {
			ENetPeer* peer;
			ENetPacket* packet;

		};
		ServerNetApp(size_t initialQueueSize) : m_inbound_queue(initialQueueSize), m_outbound_queue(initialQueueSize) {}

		~ServerNetApp() {}

		bool Start(uint16_t port);
		void Run();

		void Stop();

		moodycamel::ConcurrentQueue<ENetEvent>*		GetInboundQueue() { return &m_inbound_queue; }
		moodycamel::ConcurrentQueue<OutboundPkt>*	GetOutboundQueue() { return &m_outbound_queue; }
		
		ENetPeer* GetPeerByID(uint32_t id) {
			m_mux.lock();
			auto a = m_peers.find(id)->second;
			m_mux.unlock();
			return a;
		}



private:

		std::unordered_map<uint32_t,ENetPeer*>			m_peers;
		std::mutex										m_mux;
		moodycamel::ConcurrentQueue<ENetEvent>			m_inbound_queue;
		moodycamel::ConcurrentQueue<OutboundPkt>		m_outbound_queue;
		ENetAddress										m_enet_addr;
		ENetHost*										m_enet_host;




};