#ifndef LINKEDLIST_H
#define	LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include "mdata.h"

TCBList* CreateList(void);
TCB* Pop(TCBList* list);
TCBList* Insert(TCBList* list, TCB* newElement);
TCBList* InsertSorted(TCBList* list, TCB* newElement);
TCBList* SortList (TCBList* list);
void Remove(TCBList* list, int tid);
TCB* RemoveWaiting(TCBList* list, int waitingThread);
void DeleteList(TCBList* list);
int searchTID (TCBList* list, int tid);
int searchWaiting(TCBList* list, int tid);


#endif
