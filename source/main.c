#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <time.h>
#include <sys/time.h>

#include "config.h"
#include "header/lookup.h"
#include "header/queue.h"
#include "header/connection.h"
#include "header/client.h"
#include "header/application.h"

void segfault_sigaction(int signal, siginfo_t *si, void *arg);
void *clientRunner(void *param);
int serve();

sock server;
	
int main()
{
    // segfault catcher
    struct sigaction sa;
    memset(&sa, 0, sizeof(sigaction));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = segfault_sigaction;
    sa.sa_flags   = SA_SIGINFO;
    sigaction(SIGSEGV, &sa, NULL);

    // make daemon process
    if(DAEMON == 1) {
        // Our process ID and Session ID
        pid_t pid, sid;

        // Fork off the parent process
        pid = fork();
        if (pid < 0) {
            exit(EXIT_FAILURE);
        }

        // If we got a good PID, then we can exit the parent process.
        if (pid > 0) {
            exit(EXIT_SUCCESS);
        }

        // Change the file mode mask
        umask(0);

        // Create a new SID for the child process
        sid = setsid();
        if (sid < 0) {
            error("Can't run process");
        }

        // Change the current working directory
        if ((chdir(WORKING_DIR)) < 0) {
            error("Can't change working directory");
        }

        // Close out the standard file descriptors
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
    }

    return serve();
}

int serve()
{
    /* creating socket */
    server.id = socket(AF_INET, SOCK_STREAM, 0);
    if(server.id < 0)
        error("Error on creating socket");

    /* initializing socket */
    bzero((char *) &server.addr, sizeof(server.addr));
    server.addr.sin_family = AF_INET;
    server.addr.sin_addr.s_addr = INADDR_ANY;
    server.addr.sin_port = htons(PORT);

    /* binding socket */
    if(bind(server.id, (struct sockaddr *) &server.addr, sizeof(server.addr)) < 0)
        error("ERROR on binding socket");

    // printf("Socket created on ANY:%d.\n", PORT);

    lookupInit();
    clientInit();
    // printf("Data initialized.\n");

    lookupMaper();

    // printf("Server running.\n");
    short int cId;
    sock tmpsock;
    char *fullConnection = "1- Sorry, reach max connection. try again in later.";
    char buffer[256];

    listen(server.id, 5);
    while(1) {
        tmpsock.id = accept(server.id, (struct sockaddr *) &tmpsock.addr, &tmpsock.len);

        if(tmpsock.id == -1)
            error("Error on accepting new client");

        cId = clientGetFree();
        if(cId == -1) {
            printf("Reach max connection\n");
            bzero(buffer, 256);
            read(tmpsock.id, buffer, 255);
            write(tmpsock.id, fullConnection, strlen(fullConnection));
            close(tmpsock.id);
            continue;
        }
        clientGet(cId)->socket = tmpsock;
        clientGet(cId)->isFreeslot = 0;

        pthread_create(&(clientGet(cId)->thread), NULL, clientRunner, (void *) clientGet(cId));
    }

    // closing
    close(server.id);
    return 0;
}

void error(char *msg)
{
    char now[25];
    getTime(now);

    FILE *fp;
    fp = fopen(ERROR_LOG, "a");
    fprintf(fp, "[%s] %s\n", now, msg);
    fclose(fp);

    if(DAEMON == 0)
        printf(msg);

    exit(1);
}

void getTime(char *timing)
{
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(timing, 80, "%Y-%m-%d %H:%I:%S %p", timeinfo);
}

void *clientRunner(void *param)
{
    sclient *client = (sclient *) param;

    char bufin[256];
    char bufout[50] = "4- command not found";
    int tmp;
	char now[80];
    // printf("connected  %s\n", inet_ntoa(client->socket.addr.sin_addr));
    int lookupReturn;
    while(1) {
        bzero(bufin, 256);
        tmp = read(client->socket.id, bufin, 255);
        if(tmp == 0)
            goto clientdc;
        else if(tmp < 1) {
            // printf("Error on receiving data from %s\n", inet_ntoa(client->socket.addr.sin_addr));
            goto clientdc;
        }

		if(DAEMON == 0) {
			getTime(now);
			printf("[%s, FROM %s] %s\n", now, inet_ntoa(client->socket.addr.sin_addr), bufin);
		}
        lookupReturn = lookupCall(bufin, client);
        if(lookupReturn == 1)
            write(client->socket.id, bufout, strlen(bufout));
    }

clientdc:
    // printf("disconnect %s\n", inet_ntoa(client->socket.addr.sin_addr));
    close(client->socket.id);
    client->isFreeslot = 1;
}

void segfault_sigaction(int signal, siginfo_t *si, void *arg)
{
    char buf[50];
    sprintf(buf, "Caught segmentation fault at address %p", si->si_addr);
    error(buf);
}

pthread_mutex_t fakeMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t fakeCond = PTHREAD_COND_INITIALIZER;

void pthread_mysleep(int s)
{
	struct timespec timeToWait;
    struct timeval now;
    int rt;

    gettimeofday(&now,NULL);

    timeToWait.tv_sec = now.tv_sec + s;
    timeToWait.tv_nsec = now.tv_usec*1000;

    pthread_mutex_lock(&fakeMutex);
    rt = pthread_cond_timedwait(&fakeCond, &fakeMutex, &timeToWait);
    pthread_mutex_unlock(&fakeMutex);
}