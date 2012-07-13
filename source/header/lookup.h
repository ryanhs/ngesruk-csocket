#ifndef LOOKUP_H
#define LOOKUP_H

typedef struct {
	void (*func)(int, char **, void *);
	char *key;
} slookup;

void lookupInit();
void lookupAdd(char *key, void (*func)(int, char **, void *));
int lookupCall(char *command, void *client);

#endif