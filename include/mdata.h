/*
*	mdata.h:	arquivo de inclusão com as estruturas de dados definidas para a 
*				implementação do trabalho. Inclui a estrutura TCB (Thread Control Block)
*				e a estrutura empregada para o mutex.
*/

#ifndef __mdata__
#define	__mdata__ 1

#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	TCB TCBElement;
    TCBList * next;
} TCBList;

typedef struct {
	long int tid;
	double time;
	ucontext_t context;
} TCB;

typedef mutex {
	int flag;
	TCB * next;
} mmutex_t;

#endif