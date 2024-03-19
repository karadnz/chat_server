#include "server.h"



int get_cmd(char *command)
{
	if (!strcmp(command, "/quit"))
		return QUIT;
	if (!strcmp(command, "/ping"))
		return PING;
	if (!strcmp(command, "/topic"))
		return TOPIC;
	if (!strcmp(command, "/nick"))
		return NICK;
	if (!strcmp(command, "/msg"))
		return MSG;
	if (!strcmp(command, "/list"))
		return LIST;
	if (!strcmp(command, "/help"))
		return HELP;

	return UNKNOWN;
}

// change topic
void cmd_topic(t_server *server, char *buff_out)
{
	char *param;

	param = strtok(NULL, " ");

	if (param)
	{
		// change the topic
		pthread_mutex_lock(&(server->topic_mutex));
		server->topic[0] = '\0';
		while (param != NULL)
		{
			strcat(server->topic, param);
			strcat(server->topic, " ");
			param = strtok(NULL, " ");
		}
		pthread_mutex_unlock(&(server->topic_mutex));

		// inform everyone
		sprintf(buff_out, "<< topic changed to: %s \r\n", server->topic);
		send_message_all(server, buff_out);
	}
}

// change nick
void cmd_nick(t_server *server, t_client *client, char *buff_out)
{
	char *param;

	param = strtok(NULL, " ");

	if (param)
	{
		char *old_name = _strdup(client->name);
		if (!old_name)
		{
			ft_error("ERROR on malloc for new name");
			// contunie -> in source code
		}

		// copy and null terminate the new name
		strncpy(client->name, param, (sizeof(client->name) / sizeof(char)));
		client->name[(sizeof(client->name) / sizeof(char)) - 1] = '\0';

		// inform everyone
		sprintf(buff_out, "<< %s is known as %s\r\n", old_name, client->name);
		send_message_all(server, buff_out);

		free(old_name);
	}

	else
		send_message_self("<< name cannot ne null\r\n", client->connfd);
}

// send pm
// /msg uid message..
void cmd_msg(t_server *server, t_client *client, char *buff_out)
{
	char *param;

	param = strtok(NULL, " ");

	if (param)
	{
		int uid = atoi(param);

		param = strtok(NULL, " "); //is there a need for if (param) ?? // maybe do if (!param)
		if (param)
		{
			// extract message
			sprintf(buff_out, "[PM][%s]", client->name);
			while (param != NULL)
			{
				strcat(buff_out, " ");
				strcat(buff_out, param);
				param = strtok(NULL, " ");
			}

			// send pm to recipent 
			strcat(buff_out, "\r\n");
			send_message_client(server, buff_out, uid);
		}
		else
			send_message_self("<< message cannot be null\r\n", client->connfd);
	}

	else
		send_message_self("<< recipent uid cannot ne null\r\n", client->connfd);
}

// send a list of the active users to the client
void cmd_list(t_server *server, t_client *client, char *buff_out)
{
	sprintf(buff_out, "<< clients %d\r\n", server->client_count);
	send_message_self(buff_out, client->connfd);

	send_active_clients(server, client->connfd);
}

void cmd_help(t_client *client, char *buff_out)
{
	strcat(buff_out, "<< /quit\tQuit chatroom\r\n"); //maybe use spaces instead of \t
	strcat(buff_out, "<< /ping\tServer Test\r\n");
	strcat(buff_out, "<< /topic\t<message> Set topic\r\n");
	strcat(buff_out, "<< /nick\t<name> Change nickname\r\n");
	strcat(buff_out, "<< /msg\t<recipent uid> <message> Send PM\r\n");
	strcat(buff_out, "<< /list\tShow active clients\r\n");
	strcat(buff_out, "<< /help\tShow Help\r\n");

	send_message_self(buff_out, client->connfd);
}

int handle_cmd(t_server *server, t_client *client, char *buff_out, char *buff_in)
{
	char	*command;
	char	*param;
	int		rt;

	command = strtok(buff_in, " ");
	rt = get_cmd(command);
	
	switch (rt)
	{
	case QUIT:
		return QUIT;
		break;
	case PING:
		send_message_self("<< pong\r\n", client->connfd);
		break;
	case TOPIC:
		cmd_topic(server, buff_out);
		break;
	case NICK:
		cmd_nick(server, client, buff_out); // how to achieve contunie
		break;
	case MSG:
		cmd_msg(server, client, buff_out);
		break;
	case LIST:
		cmd_list(server, client, buff_out);
		break;
	case HELP:
		cmd_help(client, buff_out);
		break;
	case UNKNOWN:
		send_message_self("<< unknown command\r\n", client->connfd);
		break;

	default:
		//
		break;
	}

	return rt;
}