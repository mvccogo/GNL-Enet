#include <enet/enet.h>
#include <cstdio>
#include "Packet.h"
#include "NetCommands.h"
#include <chrono>
#include <iostream>

int main() {

	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		return EXIT_FAILURE;
	}

	ENetHost* client;
	client = enet_host_create(NULL, 1, 2, 0, 0);
	if (client == NULL)
	{
		fprintf(stderr,
			"An error occurred while trying to create an ENet client host.\n");
		exit(EXIT_FAILURE);
	}

	ENetAddress address;
	ENetEvent event;
	ENetPeer* peer;
	/* Connect to some.server.net:1234. */
	enet_address_set_host(&address, "26.192.37.108");
	address.port = 2004;
	/* Initiate the connection, allocating the two channels 0 and 1. */
	peer = enet_host_connect(client, &address, 2, 0);
	if (peer == NULL)
	{
		fprintf(stderr,
			"No available peers for initiating an ENet connection.\n");
		exit(EXIT_FAILURE);
	}
	int pcount = 0;
	while (enet_host_service(client, &event, 0) >= 0) {
			if (event.type == ENET_EVENT_TYPE_CONNECT) {
				printf("Connected to server! I will keep pinging now :)");
				ENetPacket* packet = enet_packet_create("", 0, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
				auto spkt = SerializablePacket(packet);

				auto millisec_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				spkt << millisec_since_epoch;
				spkt << CMD::Ping;

				enet_peer_send(peer, 0, spkt.enet_packet);
			}
			else if (event.type == ENET_EVENT_TYPE_RECEIVE) {
				SerializablePacket spkt(event.packet);
				uint16_t cmd;
				spkt >> cmd;
				switch (cmd) {
				case 6:
					{
					long long timeRec;
					std::cout << "[" << event.peer->connectID << "]: ping came back!\n";
					auto millisec_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
					spkt >> timeRec;
					long long ping = millisec_since_epoch - timeRec;
					std::cout << "Ping (round-trip): " << ping << " ms\n";
					enet_packet_destroy(spkt.enet_packet);

					}
				}


			}
	}
}
