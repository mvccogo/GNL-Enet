#include <enet/enet.h>
#include <cstdio>
#include "Server.h"

int main(int argc, char** argv) {

	Server server;


	std::thread enet_thread([&]() {
		server.Start(2004);
		});

	bool stopFlag = false;
	ENetEvent incoming_event;
	while (!stopFlag) {

		if (server.GetNetApp()->GetInboundQueue()->try_dequeue(incoming_event)) {
			switch (incoming_event.type) {
			case ENET_EVENT_TYPE_RECEIVE:
				server.OnPacket(incoming_event.peer->connectID, incoming_event.packet);
				break;
			case ENET_EVENT_TYPE_CONNECT:
				server.OnConnect(incoming_event.peer->connectID);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				server.OnDisconnect(incoming_event.peer->connectID);
				break;


			}



		}

	}




}