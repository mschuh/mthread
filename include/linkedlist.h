#ifndef LINKEDLIST_H
#define	LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include "mdata.h"

TCBList* CreateList(void);
TCB* Pop(TCBList* list);
void Insert(TCBList* list, TCB* newElement);
void InsertSorted(TCBList* list, TCB* newElement);
TCBList* SortList (TCBList* list);
TCB* Remove(TCBList* list, int waitingThread);
TCBList* DeleteList(TCBList* list);
int searchTID (TCBList* list, int tid);
int searchWaiting(TCBList* list, int tid);


#endif
