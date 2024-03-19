#include "server.h"

void queue_add(t_server *server, t_client *client)
{
	pthread_mutex_lock(&(server->clients_mutex));
	for (size_t i = 0; i < MAX_CLIENTS; i++)
	{
		if(server->clients[i])
		{
			server->clients[i] = client;
			break;
		}
	}
	pthread_mutex_unlock(&(server->clients_mutex));
}

void queue_remove(t_server *server, int uid)
{
	pthread_mutex_lock(&(server->clients_mutex));
	for (size_t i = 0; i < MAX_CLIENTS; i++)
	{
		if(server->clients[i] && server->clients[i]->uid == uid)
		{
			server->clients[i] = NULL;
			break;
		}
	}
	pthread_mutex_unlock(&(server->clients_mutex));
}