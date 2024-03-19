#include "server.h"

//
t_client *new_client(struct sockaddr_in client_addr, int connfd, int uid)
{
	t_client	*client = (t_client *)malloc(sizeof(t_client));

	client->addr = client_addr;
	client->connfd = connfd;
	client->uid = uid;

	sprintf(client->name, "%s", client->uid);
}

t_args *new_args(t_server *server, t_client *client)
{
	t_args	*args = (t_args *)malloc(sizeof(t_args));

	args->server = server;
	args->client = client;

	return (args);
}

void *handle_client(void *arg)
{
	char	buff_out[BUFFER_SZ];
	char	buff_in[BUFFER_SZ / 2];
	int		read_length;

	t_server	*server = ((t_args *)arg)->server;
	t_client	*client = ((t_args *)arg)->client;
	
	client_welcome(server, client, buff_out);

	while((read_length = read(client->connfd, buff_in, sizeof(buff_in ) - 1)) > 0)
	{
		buff_in[read_length] = '\0';
		buff_out[0] = '\0';

		strip_newline(buff_in);

		if (strlen(buff_in)) //ignore empty buffer
			continue;

		if (buff_in[0] == '/' && (handle_cmd() == 1)) //make it enum
			break;
		else
		{
			snprintf(buff_out, sizeof(buff_out),
								"[%s] %s\r\n",
								client->name,
								buff_in);
			send_message(server, buff_out, client->uid);
		}

	}

	client_goodbye(server, client, buff_out);

	pthread_detach(pthread_self());

	return NULL;
}

// t_client *client = (t_client *)malloc(sizeof(t_client));
// 		client->addr = client_addr;
// 		client->connfd = connfd;
// 		client->uid = server->uid++;
// 		sprintf(client->name, "%d", client->uid);
