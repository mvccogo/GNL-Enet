
#include "ServerNetApp.h"

bool ServerNetApp::Start(uint16_t port) {
	// This runs inside a thread!!
	m_enet_addr.host = ENET_HOST_ANY;
	m_enet_addr.port = port;
	m_enet_host = std::shared_ptr<ENetHost>(enet_host_create(&m_enet_addr, 32, 2, 0, 0));

	if (m_enet_host == NULL)
	{
		fprintf(stderr,
			"An error occurred while trying to create an ENet server host.\n");
		exit(EXIT_FAILURE);
	}

}

void ServerNetApp::Run() {



}