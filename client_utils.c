#include "server.h"

// 
t_client *new_client(struct sockaddr_in client_addr, int connfd, int uid)
{
    t_client *client = (t_client *)malloc(sizeof(t_client));

    client->addr = client_addr;
    client->connfd = connfd;
    client->uid = uid;

    sprintf(client->name, "%s", client->uid);
}


// t_client *client = (t_client *)malloc(sizeof(t_client));
// 		client->addr = client_addr;
// 		client->connfd = connfd;
// 		client->uid = server->uid++;
// 		sprintf(client->name, "%d", client->uid);
