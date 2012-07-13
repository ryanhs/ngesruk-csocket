#ifndef CONNECTION_H
#define	CONNECTION_H

typedef struct {
    int id;
    struct sockaddr_in addr;
    socklen_t len;
} sock;

#endif