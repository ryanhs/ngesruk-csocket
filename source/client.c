#include <string.h>
#include <pthread.h>
#include <netinet/in.h>

#include "config.h"
#include "header/client.h"

sclient client[MAX_CLIENT];

void clientInit()
{
	int i;
	for(i = 0; i < MAX_CLIENT; i++)
	{
		client[i].isFreeslot = 1;
	}
}

sclient *clientGet(int clientId)
{
	return &client[clientId];
}

int clientGetFree()
{
	int i;
	for(i = 0; i < MAX_CLIENT; i++)
	{
		if(client[i].isFreeslot == 1)
		{
			
			return i;
		}
	}
	
	return -1;
}