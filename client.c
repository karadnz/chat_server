#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


#include <netdb.h>

void error(const char *msg);

int main(int argc, char **argv)
{
	int sockfd;
	int port;

	char    buffer[256];

	struct sockaddr_in  server_addr;

    struct hostent  *server;

	/*
	socket(int domain, int type, int protocol)
	AF_INET     = IPv4
	SOCK_STREAM = TCP
	0           = DEFAULT
	*/
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR on opening socket");

	port = atoi(argv[2]);
    server = gethostbyname(argv[1]);
    if (server == NULL)
        error("ERROR no such host");

    
    

    //bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;		  // server byte order
	//server_addr.sin_addr.s_addr = INADDR_ANY; // auto fill with current host's IP adress
	memmove(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    server_addr.sin_port = htons(port);		  // convert port to network byte order

    if (connect(sockfd, &server_addr, sizeof(server_addr)) < 0)
        error("ERROR connecting");


	// stdin 0
	//

	struct pollfd polls[2];

	polls[0].fd = 0;		  // stdin
	polls[0].events = POLLIN; // requested events | POLLIN -> There is data to read
	polls[0].revents = 0;	  // returned events | it will be returned after the poll finished

	polls[1].fd = sockfd;	  // client
	polls[1].events = POLLIN; // requested events | POLLIN -> There is data to read
	polls[1].revents = 0;	  // returned events | it will be returned after the poll finished

	for (;;)
	{
		// fds, fd count, timeout in ms
		poll(polls, 2, 50000);

		char buffer[256] = {0};

		if (polls[0].revents & POLLIN) // to read from stdin
		{
			read(0, buffer, 255);
			send(sockfd, buffer, 255, 0); //send is  write() with additional flags
		}
		else if (polls[1].revents & POLLIN) // to read from server 
		{
			if (recv(sockfd, buffer, 255, 0) == 0) //rcv is read() with additional flags
			{
				error("Connection closed");
			}
			printf("%s\n", buffer);
		}
	}

	return (0);
}

void error(const char *msg)
{
	perror(msg);
	exit(1);
}