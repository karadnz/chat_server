#include "server.h"

// port
int main(int argc, char **argv)
{
	t_server	*server;

	struct		sockaddr_in client_addr; //temp to store accepted connection info
	int			connfd = 0;

	pthread_t	tid;

	server_init(server, atoi(argv[1]));
	server_start(server);

	while (1)
	{
		socklen_t clilen = sizeof(client_addr);
		connfd = accept(server->listenfd, &client_addr, &clilen);

		if ((server->client_count + 1) == MAX_CLIENTS)
		{
			printf("<< max cleints reached\n");
			printf("<< reject ");
			print_client_addr(client_addr);
			printf("\n");
			close(connfd);
			continue;
		}

		t_client *client = new_client(client_addr, connfd, server->uid++);

		//
		queue_add(server, client);
		pthread_create(&tid, NULL, &handle_client, (void *)client);

		sleep(1);
	}
}
