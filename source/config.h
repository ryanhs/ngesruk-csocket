#ifndef CONFIG_H
#define	CONFIG_H

// restrict configuration
#define DAEMON 0
#define WORKING_DIR "/home/ryan/switcher"
#define ERROR_LOG "data/error.log"

// server configuration
#define PORT 14045

// lookup configuration
#define MAX_LOOKUP 50

// client configuration
#define MAX_CLIENT 200

void error(char *msg);
void getTime(char *timing);
void pthread_mysleep(int s);

#endif