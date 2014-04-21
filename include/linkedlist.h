#ifndef __linkedlist__
#define	__linkedlist__ 1

#include "mdata.h";

typedef struct {
	TCB TCBElement;
    TCBList * next;
} TCBList;

TCBList* CreateList(void);
TCBList* Insert(TCBList* list, TCB* newElement);
TCBList* SortList (TCBList* list);
TCBList* Remove(TCBList* list, long int tid);
TCBList* DeleteList(TCBList* list);

#endif;