#ifndef CLIENT_H
#define	CLIENT_H

#ifndef CONNECTION_H
#include "connection.h"
#endif

typedef struct {
	short int isFreeslot;
	
    sock socket;
    pthread_t thread;
	
} sclient;

void clientInit();
sclient *clientGet(int clientId);
int clientGetFree();


#endif