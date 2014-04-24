#include "mthread.h"
#include <stdlib.h>
#include <stdio.h>


#define MAX_THREADS 500

/*
A thread main cria 500 threads para doSomeThing1. Cada oSomeThing1 cria uma thread para doSomeThing2 e aguarda o seu término.
A main aguarda o término de todas threads.
*/

void doSomeThing2();

int counterJob = 0;

void doSomeThing1(void *arg)
{

    counterJob += 1;
    int code;

    printf("\n Job %d started", counterJob);

    printf("\t Job %d running", counterJob);

    code = mcreate(doSomeThing2, NULL);

    // Verifica se houve erro ao criar a thread
    if(code < 0)
	printf("\n\terror code %d", code);

    mjoin(code);


}

void doSomeThing2()
{
    printf("\nHELLO!");
    printf("\nBYE-BYE!");
}

int main(void)
{

   int i = 0;
   int id[MAX_THREADS];


   for(i = 0; i < MAX_THREADS ; i++)
   	id[i] = mcreate(doSomeThing1, NULL);

   for(i = 0; i < MAX_THREADS ; i++)
        mjoin(id[i]);


   printf("\n main finished\n");

   return 0;

}
