#include <enet/enet.h>
#include <cstdio>

int main() {
    if (enet_initialize() != 0)
    {
        
        (stderr, "An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }
    ENetHost* client;

    client = enet_host_create(NULL, 4, 2, 0, 0);
    if (client == NULL)
    {
        fprintf(stderr,
            "An error occurred while trying to create an ENet server host.\n");
        exit(EXIT_FAILURE);
    }

    enet_host_destroy(client);
    atexit(enet_deinitialize);

	return 0;
}
