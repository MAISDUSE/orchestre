// gcc -O3 -Wall -Wextra -Wconversion -ansi -Wpedantic -Werror -std=c11 sender.c -o sender
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char* argv[])
{
	if (argc-1!=1)
	{
		fprintf(stderr, "Usage %s PORT\n", argv[0]);
		exit(10);
	}

	int v = atoi(argv[1]);
	assert(v>0 && v<65535);
	uint16_t port = (uint16_t)v;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
	assert(sockfd>=0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    assert(inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr)>=0);

    // la connexion déclenche la création du thread distant
    assert(connect(sockfd, (struct sockaddr *)&addr, sizeof(addr))>=0);

	int duration;
	assert(recv(sockfd, &duration, sizeof(duration), 0) > 0);

	printf("duration of song : %d\n", duration);


	#define BUFF_SIZE 10
	char buffer[BUFF_SIZE];
	strncpy(buffer, "test", BUFF_SIZE);
	assert(send(sockfd, buffer, sizeof(buffer), 0)!=-1);

	// la fermeture de la socket délenche la fin du thread distant
	// on reste donc "ouvert" 5 secondes pour entendre qq chose
	sleep((unsigned int) duration);

	printf("done!\n");
	close(sockfd);
	return 0;
}
