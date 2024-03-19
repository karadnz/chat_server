#include "server.h"

char *_strdup(const char *str)
{
	size_t size;
	char *rt;

	size = strlen(str) + 1;
	rt = (char *)malloc(size * sizeof(char));

	if (rt)
		memcpy(rt, str, size);

	return (rt);
}

void strip_newline(char *str)
{
	while (*str != '\0')
	{
		if (*str == '\r' || *str == '\n')
			*str = '\0';
		str++;
	}
}

void print_addr(struct sockaddr_in addr)
{
	printf("%d.%d.%d.%d",
			addr.sin_addr.s_addr & 0xff,
			(addr.sin_addr.s_addr & 0xff00) >> 8,
			(addr.sin_addr.s_addr & 0xff0000) >> 16,
			(addr.sin_addr.s_addr & 0xff000000) >> 24);
}


// move below funcs to somewhere else

// increment client counter
// inform everyone that a new client joined
// inform the client on topics
void client_welcome(t_server *server, t_client *client, char *buff_out)
{
	server->client_count++; // client count is atomic but is there problem with accesing from server ptr

	// log
	printf("<< accept ");
	print_addr(client->addr); // check how it works
	printf(" referenced by %d\n", client->uid);

	// inform everyone
	sprintf(buff_out, "<< %s has joined\r\n", client->name);
	send_message_all(server, buff_out);

	// inform client on topics
	pthread_mutex_lock(&server->topic_mutex);
	if (strlen(server->topic))
	{
		buff_out[0] = '\0';
		sprintf(buff_out, "<< topic: %s\r\n", server->topic);
		send_message_self(buff_out, client->connfd);
	}
	pthread_mutex_unlock(&server->topic_mutex);

	//
	send_message_self("<< see /help for assistance\r\n", client->connfd);

}

// close connection
// inform everyone that a client left
// remove client from queue
void client_goodbye(t_server *server, t_client *client, char *buff_out)
{
	// close connection & inform everyone
	sprintf(buff_out, "<< %s has left\r\n", client->name);
	send_message_all(server, buff_out);
	close(client->connfd);

	// remove client & yield thread
	queue_remove(server, client->uid);
	printf("<< quit");
	print_addr(client->addr);
	printf(" referenced by %d\n", client->uid);

	free(client);
	server->client_count--;

}

