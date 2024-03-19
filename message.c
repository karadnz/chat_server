#include "server.h"

// send to all clients except the sender
void send_message(t_server *server, char *msg, int uid)
{
	pthread_mutex_lock(&(server->clients_mutex));
	for (size_t i = 0; i < MAX_CLIENTS; i++)
	{
		if (server->clients[i] && server->clients[i]->uid != uid)
		{
			if (write(server->clients[i]->connfd, msg, strlen(msg)) < 0)
				ft_error("ERROR on writing to connfd");
		}
	}
	pthread_mutex_unlock(&(server->clients_mutex));
}

// send to all clients
void send_message_all(t_server *server, char *msg)
{
	pthread_mutex_lock(&(server->clients_mutex));
	for (size_t i = 0; i < MAX_CLIENTS; i++)
	{
		if (server->clients[i])
		{
			if (write(server->clients[i]->connfd, msg, strlen(msg)) < 0)
				ft_error("ERROR on writing to connfd");
		}
	}
	pthread_mutex_unlock(&(server->clients_mutex));
}

// send to sender
void send_message_self(char *msg, int connfd)
{
	// why no mutex and why we didnt send the client
	if (write(connfd, msg, strlen(msg)) < 0)
		ft_error("ERROR on writinf to connfd");
}

// send to client
void send_message_client(t_server *server, char *msg, int uid)
{
	pthread_mutex_lock(&(server->clients_mutex));
	for (size_t i = 0; i < MAX_CLIENTS; i++)
	{
		if (server->clients[i] && server->clients[i]->uid != uid)
		{
			if (write(server->clients[i]->connfd, msg, strlen(msg)) < 0)
				ft_error("ERROR on writing to connfd");
		}
	}
	pthread_mutex_unlock(&(server->clients_mutex));
}

// send list of active clients
void send_active_clients(t_server *server, int connfd)
{
	char rt[64];

	pthread_mutex_lock(&(server->clients_mutex));
	for (size_t i = 0; i < MAX_CLIENTS; i++)
	{
		if (server->clients[i])
		{
			sprintf(rt, "<< [%d] %s\r\n",
					server->clients[i]->uid,
					server->clients[i]->name);
			send_message_self(rt, connfd);
		}
	}
	pthread_mutex_unlock(&(server->clients_mutex));
}