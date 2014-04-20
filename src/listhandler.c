TCBList* CreateList(void)
{
       return NULL;
}

TCBList* Insert(TCBList* list, TCB newElement)
{
       TCBList *new; //new element

       new = (TCBList*) malloc(sizeof(TCBList)); //allocates the new node
       new->TCBElement = newElement; //inserts the information of the new node    
       List->next = new; //inserts the new node at the beggining of the list

       return list;
}   

TCBList* SortList (void) //Sort by execTime, TO DO
{

}

TCBList* Remove(TCBList* list, long int tid)
{
     TCBList *prev = NULL; //auxiliar pointer to the previous position
     TCBList *ptaux = list; //auxiliar pointer to run through the list

     //search for the element in the list
     while (ptaux !=NULL && (ptaux->TCBElement.tid == tid))
     {          
          prev = ptaux;
          ptaux = ptaux->prox;
     }
     
     //verify if the element has been found

     if (ptaux == NULL) //the element doesn't exist
       return list; //returns the original list
       
    if (prev == NULL) //the first element will be removed
      list = ptaux->prox;

    else //it'll remove from the middle or the end of the list
      prev->prox = ptaux->prox;
      
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