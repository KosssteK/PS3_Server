#include <enet/enet.h>
#include <iostream>
#include <vector>
#include <string>


std::string getOutputData(char* data);

int main(int argc, char ** argv)
{
	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	ENetAddress address;
	ENetHost * server;
	/* Bind the server to the default localhost.     */
	/* A specific host address can be specified by   */
	/* enet_address_set_host (& address, "x.x.x.x"); */
	address.host = ENET_HOST_ANY;
	/* Bind the server to port 1234. */
	address.port = 1234;
	server = enet_host_create(&address /* the address to bind the server host to */,
		32      /* allow up to 32 clients and/or outgoing connections */,
		2      /* allow up to 2 channels to be used, 0 and 1 */,
		0      /* assume any amount of incoming bandwidth */,
		0      /* assume any amount of outgoing bandwidth */);
	if (server == NULL)
	{
		fprintf(stderr,
			"An error occurred while trying to create an ENet server host.\n");
		exit(EXIT_FAILURE);
	}


	ENetEvent event;
	//std::cout << (enet_host_service(server, &event, 10000)) << std::endl;
	std::cout << "start server !@" << std::endl;
	ENetPacket * packet;
	std::string outputData;
	std::string aa;
	enet_uint32 connectID;
	/* Wait up to 1000 milliseconds for an event. */
	while (enet_host_service(server, &event, 1000) >= 0)
	{
		//printf("A new client connected from %x:%u.\n");
		//std::cout << server->connectedPeers << std::endl;
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			connectID = event.peer->connectID;
			aa = std::to_string(event.peer->connectID);
			std::cout << "A new client connected from " << event.peer->address.host << " | " << aa << " | " << connectID << std::endl;
			/* Store any relevant client information here. */


			outputData = "N:" + std::to_string(server->connectedPeers) + std::string(":");
			std::cout << server->connectedPeers << std::endl;
			for (int i = 0; i < server->connectedPeers; i++) {
				outputData += std::to_string(server->peers[i].connectID);
				outputData += ":";
			}
			//std::cout << "   final data: " << outputData << std::endl;
			packet = enet_packet_create(outputData.c_str(),
				strlen(outputData.c_str()) + 1,
				ENET_PACKET_FLAG_RELIABLE);
			enet_host_broadcast(server, 0, packet);
			break;
		case ENET_EVENT_TYPE_RECEIVE:

			outputData = getOutputData((char*)event.packet->data);// +server->connectedPeers;

			packet = enet_packet_create(outputData.c_str(),
				strlen(outputData.c_str()) + 1,
				ENET_PACKET_FLAG_RELIABLE);
			enet_host_broadcast(server, 0, packet);
			/* Clean up the packet now that we're done using it. */
			enet_packet_destroy(event.packet);

			break;

		case ENET_EVENT_TYPE_DISCONNECT:
			printf("%s disconnected.\n", event.peer->data);
			/* Reset the peer's client information. */
			event.peer->data = NULL;
		}
	}


	std::cout << "end" << std::endl;
	int a;
	std::cin >> a;

	//enet_host_destroy(client);
	enet_host_destroy(server);


}

std::string getOutputData(char* data) {
	return data;
}