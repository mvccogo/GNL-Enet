#include <enet/enet.h>
#include <cstdio>
#include "Server.h"

int main(int argc, char** argv) {

	Server server(2004);


	std::thread enet_thread([&]() {
		server.Start();
		});

	bool stopFlag = false;
	ENetEvent incoming_event;
	while (!stopFlag) {

		
		bool test = server.GetNetApp()->GetInboundQueue()->try_dequeue(incoming_event);
		if (test && incoming_event.type == ENET_EVENT_TYPE_RECEIVE) {
			server.OnPacket(incoming_event.peer->connectID, incoming_event.packet);
		}
	


	}






}