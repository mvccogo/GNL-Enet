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
	unsigned long long ull_cur_tick;
	unsigned long long ull_last_tick = 0;
	unsigned long long ull_run_tick;

	while (!stopFlag) {

		ull_cur_tick = GetTickCount64();
		// Process packets at 15ms intervals.
		if (ull_cur_tick - ull_last_tick > 15) {
			while (server.GetNetApp()->GetInboundQueue()->try_dequeue(incoming_event)) {
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
		ull_cur_tick = GetTickCount64();
		ull_run_tick = ull_cur_tick - ull_last_tick;
		server.GameLoop(ull_run_tick);
		ull_last_tick = GetTickCount64();

		//}

	}
}