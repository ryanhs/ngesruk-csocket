#include <malloc.h>
#include <string.h>

#include <stdio.h>

#include "config.h"
#include "header/lookup.h"

slookup lookup[MAX_LOOKUP];
int lookupCounter;

void lookupInit()
{
    lookupCounter = 0;
}

void lookupAdd(char *key, void (*func)(int, char **, void *))
{
    lookup[lookupCounter].key = key;
    lookup[lookupCounter].func = func;
    lookupCounter++;
}

int lookupCall(char *command, void *client)
{
    int approx_arg = 25;
    char *key;
    char *tmp = calloc(strlen(command) + (approx_arg * 2), sizeof(char));
    char *tmp_free = tmp;
    char **argv = calloc(approx_arg, sizeof(char **));
    char **argv_free = argv;
    int argc = 0;

    char delimiter = ' ';
    int i = 0;

    char *cmd = command;
    argv[argc] = tmp;
    while(*cmd != '\0') {
        if(i > 254) goto lookup_callreturn;

        /*
		if(*cmd == '\'') {
				*tmp++ = '\\';
				*tmp++ = *cmd++;
			} else 
		*/
		if(*cmd == '\'') {
			if(*(cmd - 1) != '\\') {
				*tmp++ = '\\';
				*tmp++ = *cmd++;
			}
        }
		
		if(*cmd == delimiter) {
            cmd++;
            *tmp++ = '\0';
            argc++;
            argv[argc] = tmp;
            i++;
        } else
            *tmp++ = *cmd++;

        i++;
    }
    *tmp = '\0';

    key = argv[0];
    argv++;

    /*
    printf("func: %s\n", key);
    printf("argc: %d\n", argc);
    for(i = 0; i < argc; i++) {
    	printf("   argv[%d]: %s\n", i, argv[i]);
    }
    return 0;
    */

    int rc = 1;
    for(i = 0; i < lookupCounter; i++) {
        if(strcmp(lookup[i].key, key) == 0) {
            lookup[i].func(argc, argv, client);
            rc = 0;
        }
    }

lookup_callreturn:
    free(tmp_free);
    free(argv_free);

    return rc;
}
