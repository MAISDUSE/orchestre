#include "musician.h"
#include "sockets/musician_sockets.h"

int main()
{
    uint16_t port = 1234;

    initSocket(port);
    connectSocket();

	printf("done!\n");
	return 0;
}
