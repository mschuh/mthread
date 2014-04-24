//*******************************************************************
//                                                                  *
//              Universidade Federal do Rio Grande do Sul           *
//                   Instituto de Informática                       *
//                                                                  *
//                 INF 01142 - Sistemas Operacionais I              *
//                                                                  *
//               libmthread - Gerenciador de threads N:1           *
//                                                                  *
//                     Felipe Nogueira - 219827                     *
//                    Matheus Schuh - 219824                        *
//                                                                  *
//*******************************************************************



//************************** INCLUDES *******************************



#include "../include/linkedlist.h"
#include <stdio.h>
#include <stdlib.h>



//************* MAIN FUNCTIONS PRESENTED IN HEADER ******************



TCBList* CreateList(void)
{
       return NULL;
}

<<<<<<< HEAD
//Print nodes by TID and ExecTime
void PrintList(TCBList* list)
{
    TCBList* node;
    node=list;
    printf("\nList Nodes\n");
    while(node!=NULL)
    {
        printf("TID: %i ", node->TCBElement->tid);
        printf("ExecTime: %f ", node->TCBElement->execTime);
        node=node->next;
    }
}

//Returns the first element from the list removing it
=======
>>>>>>> FETCH_HEAD
TCB* Pop(TCBList** list)
{
    TCB* popped = (TCB*)malloc(sizeof(TCB));
    TCBList* ptaux = *list;

    if (ptaux == NULL) //If list is empty
        return NULL;

    popped = ptaux->TCBElement;
    *list = ptaux->next;

    free(ptaux);

    return popped;
}

<<<<<<< HEAD
//Insert the element in the first position of the list
TCBList* Insert(TCBList* list, TCB* newElement)
=======
TCBList* InsertLast(TCBList* list, TCB* newElement) //inserts a new node in the end of the list
>>>>>>> FETCH_HEAD
{
       TCBList *new_node; //new element

       new_node = (TCBList*) malloc(sizeof(TCBList)); //allocates the new node
       new_node->TCBElement = newElement; //inserts the information of the new node

       new_node->next = list;
       list = new_node;
       
<<<<<<< HEAD
       return list;
}

//Insert the element in the correct place by execTime, keeping the list sorted
=======
       return list;	
}

>>>>>>> FETCH_HEAD
TCBList* InsertSorted(TCBList* list, TCB* newElement)
{
       TCBList *new_node; //new element
       TCBList *ant = NULL; //auxiliar pointer to the previous position
       TCBList *ptaux = list; //auxiliar pointer to run through the list

       //allocates the new node
       new_node = (TCBList*) malloc(sizeof(TCBList));

       //inserts the information of the new node
       new_node->TCBElement = newElement;

       //search for the right position
       while ((ptaux!=NULL) && (ptaux->TCBElement->execTime < newElement->execTime))
       {       
    	     ant = ptaux;
    	     ptaux = ptaux->next;
       }

       //insert the element
       if (ant == NULL) //first position
       {
               new_node->next = list;
               list = new_node;
       }
       else
       {
            new_node->next = ant->next;
            ant->next = new_node;
       }
       
       return list;
}

<<<<<<< HEAD
//Remove an element searching for it by its waitingThread field
=======
TCBList* Insert(TCBList* list, TCB* newElement)
{
       TCBList *new_node; //new element

       new_node = (TCBList*) malloc(sizeof(TCBList)); //allocates the new node
       new_node->TCBElement = newElement; //inserts the information of the new node

       new_node->next = list;
       list = new_node;
       
       return list;
}

>>>>>>> FETCH_HEAD
TCB* RemoveWaiting(TCBList** list, int waitingThread)
{
     TCB* removed = (TCB*)malloc(sizeof(TCB));
     TCBList *prev = NULL; //auxiliar pointer to the previous position
     TCBList *ptaux = *list; //auxiliar pointer to run through the list

     //search for the element in the list
     while (ptaux !=NULL && (ptaux->TCBElement->waitingThread != waitingThread))
     {
          prev = ptaux;
          ptaux = ptaux->next;
     }

     //verify if the element has been found

     if (ptaux == NULL) //the element doesn't exist
       return NULL; //returns the original list

    if (prev == NULL) //the first element will be removed
      *list = ptaux->next;

    else //it'll remove from the middle or the end of the list
      prev->next = ptaux->next;

    removed = ptaux->TCBElement ;
    free(ptaux); //frees the allocated memory

    return removed;
}

<<<<<<< HEAD
//Remove an element searching for it by its tid field
=======
>>>>>>> FETCH_HEAD
void Remove(TCBList** list, int tid)
{
     TCBList *prev = NULL; //auxiliar pointer to the previous position
     TCBList *ptaux = *list; //auxiliar pointer to run through the list

     //search for the element in the list
     while (ptaux !=NULL && (ptaux->TCBElement->tid != tid))
     {
          prev = ptaux;
          ptaux = ptaux->next;
     }

     //verify if the element has been found

    if (prev == NULL) //the first element will be removed
      *list = ptaux->next;

    else //it'll remove from the middle or the end of the list
      prev->next = ptaux->next;
      
      free(ptaux);
}

//Returns 0 if the element with the tid parameter is found in the list. Otherwise, returns -1
int searchTID (TCBList* list, int tid)
{
    TCBList* ptaux = list;
    while (ptaux != NULL)
    {
        if(ptaux->TCBElement->tid == tid)
            return SUCESS_CODE;
        ptaux = ptaux->next;
    }

    return ERROR_CODE;
}

//Returns 0 if the element with the waitingThread parameter is found in the list. Otherwise, returns -1
int searchWaiting(TCBList* list, int waitingTid)
{
    TCBList* ptaux = list;
    while (ptaux != NULL)
    {
        if(ptaux->TCBElement->waitingThread == waitingTid)
            return SUCESS_CODE;
        ptaux = ptaux->next;
    }

    return ERROR_CODE;
}

void DeleteList(TCBList* list)
{
   TCBList *ptaux; //auxiliar pointer to run through the list

   while (list != NULL)
   {
         ptaux = list;
         list = list->next;
         free(ptaux);
   }
   free(list);
}

int mmutex_init(mmutex_t *mutex) //initializes a mutex variable
{
	mutex->flag = 0;
	mutex->listMutex = NULL;
	
	return SUCESS_CODE;
}
int mlock (mmutex_t * mutex)
{
	if(mutex->flag == 0) //if critical section is free
	{
		mutex->flag = 1; //now critic section is locked
		return SUCESS_CODE;
	}
	else //if critical section is locked
	{
		runningThread->state = BLOCKED;
		runningThread->waitingThread = -1;
		InsertLast(mutex->listMutex, runningThread);

		runningThread->contextFlag = 0;
		getcontext(&(runningThread->context)); //save the context because the thread was blocked
		if (runningThread->contextFlag == 0)
		{
			runningThread->contextFlag = 1;
        		if (Scheduler() == ERROR_CODE)
				return ERROR_CODE;
		}
	}
	
	return SUCESS_CODE;
}
int munlock (mmutex_t *mutex)
{
	mutex->flag = 0; //unlock the critical section
	
	TCB* mutexThread = (TCB*)malloc(sizeof(TCB));
	
	if (mutex->listMutex != NULL) //pops of the list the next thread allowes to use the critical section
	{	
		mutexThread = Pop(&mutex->listMutex);
		mutexThread->state = READY;
	}

	return SUCESS_CODE;
}
