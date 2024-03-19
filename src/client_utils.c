#include "server.h"

//
t_client *new_client(struct sockaddr_in client_addr, int connfd, int uid)
{
	t_client	*client = (t_client *)malloc(sizeof(t_client));

	client->addr = client_addr;
	client->connfd = connfd;
	client->uid = uid;

	sprintf(client->name, "%d", client->uid);

	return (client);
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
	char	buff_out[BUFFER_SZ]; //maybe store them in client or other struct
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
		//printf("incoming msg from %s: %s\n",client->name, buff_in);

		if (!strlen(buff_in)) //ignore empty buffer
			continue;

		if (buff_in[0] == '/' && (handle_cmd(server, client, buff_out, buff_in) == QUIT)) //handle commands if present
			break;
		else
		{
			snprintf(buff_out, sizeof(buff_out), "[%s] %s\r\n", client->name, buff_in);
			//printf("outgoing msg from %s: %s\n",client->name, buff_out);
			send_message(server, buff_out, client->uid);
		}

	}

	client_goodbye(server, client, buff_out);

	pthread_detach(pthread_self());

	return NULL;
}

