#ifndef SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>
#include <sys/types.h>
#include <signal.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define MAX_CLIENTS 100
#define BUFFER_SZ 2048

typedef struct	s_client
{
	struct sockaddr_in addr;
	int connfd;
	int uid;
	char name[32];
} t_client;

typedef struct	s_server
{
	_Atomic unsigned int client_count;
	int uid;
	t_client *clients[MAX_CLIENTS];

	struct sockaddr_in server_addr;
	int listenfd;

	pthread_mutex_t clients_mutex;
	pthread_mutex_t topic_mutex;

	char topic[BUFFER_SZ / 2];

} t_server;


typedef struct	s_args
{
	t_server	*server;
	t_client	*client;
} t_args;

// utils
void ft_error(const char *msg);
char *_strdup(const char *s);
void strip_newline(char *s);
void print_client_addr(struct sockaddr_in addr);

// server
void init_server(t_server *server, int port);
void start_server(t_server *server);

void client_welcome(t_server *server, t_client *client, char *buff_out);
void client_goodbye(t_server *server, t_client *client, char *buff_out);


// queue +
void queue_add(t_server *server, t_client *client); // add client to queue
void queue_remove(t_server *server, int uid);

// message +
void send_message(t_server *server, char *msg, int uid);		// send to all clients except the sender
void send_message_all(t_server *server, char *msg);				// send to all clients
void send_message_self(char *msg, int connfd);					// send to sender
void send_message_client(t_server *server, char *msg, int uid); // send to client
void send_active_clients(t_server *server, int connfd);			// send list of active clients

// client
t_client	*new_client(struct sockaddr_in client_addr, int connfd, int uid);
t_args		*new_args(t_server *server, t_client *client);
void		*handle_client(void *arg);

#endif