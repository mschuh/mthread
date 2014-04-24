//*******************************************************************
//                                                                  *
//              Universidade Federal do Rio Grande do Sul           *
//                   Instituto de Informática                       *
//                                                                  *
//                 INF 01142 - Sistemas Operacionais I              *
//                                                                  *
//               libmthread - Gerenciador de threads N:1            *
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

//Insert the element in the first position of the list
TCBList* Insert(TCBList* list, TCB* newElement)
{
       TCBList *new_node; //new element

       new_node = (TCBList*) malloc(sizeof(TCBList)); //allocates the new node
       new_node->TCBElement = newElement; //inserts the information of the new node

       new_node->next = list;
       list = new_node;
       
       return list;
}

TCBList* InsertLast(TCBList* list, TCB* newElement)
{
       TCBList *new_node; //new element
	TCBList *ptaux = list;

	new_node = (TCBList*) malloc(sizeof(TCBList)); //allocates the new node
	new_node->TCBElement = newElement; //inserts the information of the new node
	
	if (ptaux == NULL) //empty list
	{
		list = new_node;
		list->next = NULL;
		return list;
	}

	while(ptaux->next != NULL)
		ptaux = ptaux->next;
	ptaux->next = new_node;
	new_node->next = NULL;

	return list;	
}

//Insert the element in the correct place by execTime, keeping the list sorted
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
       while ((ptaux!=NULL) && (ptaux->TCBElement->execTime <= newElement->execTime))
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

//Remove an element searching for it by its waitingThread field
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

//Remove an element searching for it by its tid field
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
