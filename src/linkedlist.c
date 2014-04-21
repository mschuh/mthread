TCBList* CreateList(void)
{
       return NULL;
}

TCBList* Insert(TCBList* list, TCB* newElement)
{
       TCBList *new_node; //new element

       new_node = (TCBList*) malloc(sizeof(TCBList)); //allocates the new node
       new_node->TCBElement = newElement; //inserts the information of the new node

       new_node->next = list;
       list = new_node;

       return list;
}

TCBList* Remove(TCBList* list, long int tid)
{
     TCBList *prev = NULL; //auxiliar pointer to the previous position
     TCBList *ptaux = list; //auxiliar pointer to run through the list

     //search for the element in the list
     while (ptaux !=NULL && (ptaux->TCBElement.tid != tid))
     {          
          prev = ptaux;
          ptaux = ptaux->next;
     }
     
     //verify if the element has been found

     if (ptaux == NULL) //the element doesn't exist
       return list; //returns the original list
       
    if (prev == NULL) //the first element will be removed
      list = ptaux->next;

    else //it'll remove from the middle or the end of the list
      prev->next = ptaux->next;
      
    free(ptaux); //frees the allocated memory
    
    return list;
}  
 
TCBList* DeleteList(TCBList* list)
{
   TCBList *ptaux; //auxiliar pointer to run through the list

   while (list != NULL)
   {
         ptaux = list;
         list = list->prox;
         free(ptaux);
   }
   free(list);   
   return NULL;            
}   

//Finds length of list, which is usefull in selecting a random pivot
int ListLength (TCBList* list)
{
    TCBList *ptaux = list;

    int i=0;
    while(ptaux!=NULL)
    {
        i++;
        ptaux = ptaux->next;
    }
    return i;
}

// Selects a random pivot point
TCBList* SelectPivot(TCBList* list)
{
    int j, n, i;
    n = ListLength(list);

    TCBList* pivot = list;

    j = rand()%n;

    for (i = 0; i < j; ++i)
    {
        pivot = pivot->next;
    }

    return pivot;
}

TCBList* SortList (TCBList* list) //Sort by execTime
{
    int flag = 0;
  // Return NULL list
    if (ListLength(list) <= 1)
  return list;

    TCBList *less=NULL, *more=NULL, *next, *end, *ptaux = NULL;

    // Select a random pivot point
    TCBList* pivot = SelectPivot(list);

    // Remove pivot from list
    while(list !=NULL)
    {
        next = list->next;

        if (list != pivot) //compares by the adress
        {
            list->next = ptaux;
            ptaux = list;
        }
        
        list = next;
    }

    // Divide & Conq
    while(ptaux != NULL)
    {
        next = ptaux->next;

        if(ptaux->TCBElement.execTime <= pivot->TCBElement.execTime)
        {
            ptaux->next = less;
            less = ptaux;
        }
        else
        {
            ptaux->next = more;
            more = ptaux;
        }
        ptaux = next;
    }

    // Recursive Calls
    less = SortList(less);
    more = SortList(more);

    // Merge
    if(less != NULL)
    {
        end = less;
        while(end->next != NULL)
        {
            end = end->next;
        }
        pivot->next = more;
        end->next = pivot;

        return less;
    }
    else
    {
        pivot->next = more;
        return pivot;
    }
}