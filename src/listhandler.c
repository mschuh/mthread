TCBList* listcreate(void)
{
       return NULL;
}

TCBList* insert(TCBList* List, TCB Element)
{
       TCBList *new; //new element

       new = (TCBList*) malloc(sizeof(TCBList)); //allocates the new node
       new->TCBElement = Element; //inserts the information of the new node    
       List->next = new; //inserts the new node at the beggining of the list

       return List;
}   

TCBList* remove((TCBList* List, long int tid)
{
     TipoPtNo *ant = NULL; //ponteiro auxiliar para a posiÁ„o anterior
     TipoPtNo *ptaux = ptLista; //ponteiro auxiliar para percorrer a lista

     /*procura o elemento na lista*/
     while (ptaux !=NULL && (strcmp(ptaux->info.titulo, titulo)))
     {          
          ant = ptaux;
          ptaux = ptaux->prox;
     }
     
     /*verifica se achou*/
     if (ptaux == NULL)
       return ptLista; /*retorna a lista original*/
       
    if (ant == NULL) /*vai remover o primeiro elemento*/
      ptLista = ptaux->prox;
    else /*vai remover do meio ou do final*/
      ant->prox = ptaux->prox;
      
    free(ptaux); /*libera a memÛria alocada*/
    
    return ptLista;
}  
 
TipoPtNo* destroi(TipoPtNo* ptLista)
{
   TipoPtNo *ptaux; //ponteiro auxiliar para percorrer a lista
   while (ptLista != NULL)
   {
         ptaux = ptLista;
         ptLista = ptLista->prox;
         free(ptaux);
   }
   free(ptLista);   
   return NULL;            
}   


void imprimeInv(TipoPtNo* ptLista)
{  
     TipoPtNo* ptaux;
     int numelem, i;
     
      numelem=0;
      ptaux = ptLista;
      
      while (ptaux!=NULL)
       {
             ptaux = ptaux->prox;
             numelem++;
       }
       
      while (numelem>=1)
        {
            ptaux = ptLista;
            i=1;
            while(i<numelem)
             {
                 i++;
                 ptaux = ptaux->prox;
             }
             printf("Titulo = %s Distribuidor = %s Diretor = %s\n",ptaux->info.titulo,ptaux->info.distr,ptaux->info.diretor);
             numelem--;
        }    
}    

void imprimeRec(TipoPtNo* ptLista)
{  

     if (ptLista != NULL)
     {
     imprimeRec(ptLista->prox);
     printf("Titulo = %s Distribuidor = %s Diretor = %s\n",ptLista->info.titulo,ptLista->info.distr,ptLista->info.diretor);
    
     }
}    
