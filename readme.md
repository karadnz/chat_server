# Chat server 

This is a simple chat server i developed to better understand sockets.

It supports:

- multiple clients
- basic commands
- custom port
  
to run the server:

`./server port`

to run the client(you have to compile it yourself):

`./client localhost port`

## UML Diagram

![diagram](/docs/diagram.png "diagram")

## Commands

- `/quit` : Quit chatroom.
- `/ping` : Test.
- `/topic <message>` : Set topic.
- `/nick <nick>` : Change nick.
- `/msg <uid>` : Send PM.
- `/list` : Show active clients.
- `/help` : Show commands.

## Important Functions

### socket()
`int socket (int family, int type, int protocol);`

creates a socket and returns an fd for it

### bind()
`struct sockaddr_in serv; /* IPv4 socket address structure */
int bind(sockfd, (struct sockaddr*) &serv, sizeof(serv))`

bind the socket with an ip and port.

### listen()
`int listen(int sockfd, int backlog);`

tell kernel to accept incoming connection requests to the socket.

### accept()
`int socket (int family, int type, int protocol);`

accept a request and create a new socket for it.

## Why thread over process to handle clients?

- less memory usage.
- easier IPC(inter process communication).

## Useful links

https://www.cs.dartmouth.edu/~campbell/cs50/socketprogramming.html

more will be added
