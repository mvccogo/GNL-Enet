#include <enet/enet.h>
#include <cstdio>
#include "Server.h"

int main(int argc, char** argv) {

	Server server(2004);


	std::thread enet_thread([&]() {
		server.Start();
		});

	bool stopFlag = false;
	while (!stopFlag) {

		ENetEvent incoming_event;
		bool test = server.GetNetApp()->GetInboundQueue()->try_dequeue(incoming_event);
		if (test) {
			server.OnPacket(incoming_event.peer->connectID, *incoming_event.packet);

		}
	


	}






}