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
TCB* Remove(TCBList* list, int waitingThread);
TIDList* Remove_tid(TIDList* list, int tid)
TCBList* DeleteList(TCBList* list);

#endif
