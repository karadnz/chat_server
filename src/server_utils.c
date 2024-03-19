#include "server.h"

void ft_error(const char *msg)
{
	perror(msg);
	exit(1);
}

// init the server struct
t_server *new_server(int port)
{
	t_server *server;

	server = (t_server *)malloc(sizeof(t_server));
	
	server->client_count = 0;
	server->uid = 0;

	server->listenfd = 0;

	server->server_addr.sin_family = AF_INET;
	server->server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server->server_addr.sin_port = htons(port);

	
	pthread_mutex_init(&(server->clients_mutex), NULL);
	pthread_mutex_init(&(server->topic_mutex), NULL);

	return (server);

}

// create a socket
// bind it
// listen it
void server_start(t_server *server)
{
	server->listenfd = socket(AF_INET, SOCK_STREAM, 0);

	if (server->listenfd < 0)
		ft_error("ERROR on opening socket");

	//signal(SIGPIPE, SIG_IGN); // ignore pipe signals

	if (bind(server->listenfd, (struct sockaddr *)&server->server_addr, sizeof(server->server_addr)) < 0)
		ft_error("ERROR on socket binding");

	//The listen function places a socket in a state in which it is listening for an incoming connection.
	if (listen(server->listenfd, 10) < 0)
		ft_error("ERROR on socket listening");
	
	printf("<[ SERVER STARTED ]>\n");
}