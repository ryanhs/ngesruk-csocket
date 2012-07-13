#ifndef QUEUE_H
#define	QUEUE_H

typedef struct {
	int jobInt;
	void *jobVoid;
	char from[25];
	void *next;
} QueueElement;

typedef struct {
	short int leftover;
	QueueElement *next;
	QueueElement *last;
} Queue;

Queue queueCreate();
void queueAdd(Queue *Q, void *jobVoid, int jobInt, char *from);
QueueElement *queueGet(Queue *Q);
void queuePurge(Queue *Q);


#endif