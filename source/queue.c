#include <malloc.h>
#include <string.h>
#include "header/queue.h"

Queue queueCreate()
{
    Queue Q;
    Q.leftover = 0;
    Q.next = NULL;
    Q.last = NULL;
    return Q;
}

void queueAdd(Queue *Q, void *jobVoid, int jobInt, char *from)
{
    QueueElement *Qe;
    Qe = malloc(sizeof(QueueElement));
    Qe->jobVoid = jobVoid;
    Qe->jobInt = jobInt;
    Qe->next = NULL;
    strcpy(Qe->from, from);

    if(Q->next == NULL) {
        Q->next = Qe;
        Q->last = NULL;
    }

    if(Q->last != NULL)
        Q->last->next = Qe;

    Q->last = Qe;
    Q->leftover++;
}

QueueElement *queueGet(Queue *Q)
{
    if(Q->leftover == 0)
        return NULL;

    QueueElement *tmp;
    tmp = Q->next;

    if(Q->next->next != NULL)
        Q->next = (QueueElement *) Q->next->next;

    if(Q->leftover == 1)
        Q->next = NULL;

    Q->leftover--;
    return tmp;
}

void queuePurge(Queue *Q)
{
	QueueElement *tmp;
	QueueElement *tmp1;
	
	if(Q->leftover > 0)
	{
		tmp = Q->next;
		while(tmp != NULL)
		{
			tmp1 = tmp->next;
			free(tmp);
			tmp = tmp1;
		}
	}
	
    Q->leftover = 0;
    Q->next = NULL;
    Q->last = NULL;
}