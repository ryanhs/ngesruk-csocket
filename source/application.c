#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "config.h"
#include "header/lookup.h"
#include "header/queue.h"
#include "header/connection.h"
#include "header/client.h"

/* function initializing */
void client_ping(int argc, char **argv, void *clientRaw);
void client_ehlo(int argc, char **argv, void *clientRaw);

// do initializing, invoke with lookup
void lookupMaper()
{
    lookupAdd("PING", &client_ping);
    lookupAdd("EHLO", &client_ehlo);
	
}

void client_ping(int argc, char **argv, void *clientRaw)
{
	sclient *client = clientRaw;
	write(client->socket.id, "1", strlen("1"));
}

void client_ehlo(int argc, char **argv, void *clientRaw)
{
    sclient *client = clientRaw;
    char buf[50];

    sprintf(buf, "ELHO BACK");
    write(client->socket.id, buf, strlen(buf));
}